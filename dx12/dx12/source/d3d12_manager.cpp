#include "../header/d3d12_manager.h"
#include "../header/utility.h"

using namespace Microsoft::WRL;

void D3D12Manager::Initialize(HWND _hwnd)
{
	UINT dxgiflags = 0;
#if _DEBUG
	dxgiflags |= DXGI_CREATE_FACTORY_DEBUG;
	CreateDebugLayer();
#endif
	CreateFactory(dxgiflags);

	CreateDevice(dxgiflags);
#if _DEBUG
	CreateInfoQueue();
#endif
	CreateCommandBuffer();

	CreateSwapChain(_hwnd);

	CreateDescriptorHeap();

	CreateRenderTargetView();

	CreateDepthStencilView();

	CreateFence();

	SetupViewport();
}

void D3D12Manager::RenderBegin()
{
	// 現在のフレームインデックス取得
	frame_buffer_index = swap_chain4->GetCurrentBackBufferIndex();

	// コマンドリセット
	command_buffer.allocators.at(frame_buffer_index)->Reset();
	command_buffer.list->Reset(command_buffer.allocators.at(frame_buffer_index).Get(), nullptr);

	// 描画切り替え
	BarrierTranslation(RTV.buffer.at(frame_buffer_index).Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// 画面のクリア
	ScreenClear();

	// ビューポートの設定
	SetViewport();
}

void D3D12Manager::RenderEnd()
{
	// 描画切り替え
	BarrierTranslation(RTV.buffer.at(frame_buffer_index).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	command_buffer.list->Close();

	// 実行
	ID3D12CommandList* command_list[] = { command_buffer.list.Get() };
	command_buffer.queue->ExecuteCommandLists(_countof(command_list), command_list);

	swap_chain4->Present(0, 0);

	WaitFence();
}

void D3D12Manager::BarrierTranslation(ID3D12Resource* _resource, D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _after)
{
	D3D12_RESOURCE_BARRIER barrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = _resource,
			.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			.StateBefore = _before,
			.StateAfter = _after
		}
	};
	command_buffer.list->ResourceBarrier(1, &barrier);
}

void D3D12Manager::ScreenClear()
{
	D3D12_CPU_DESCRIPTOR_HANDLE RTH = RTV.handle.at(frame_buffer_index);
	D3D12_CPU_DESCRIPTOR_HANDLE DSH = DSV.handle;
	command_buffer.list->OMSetRenderTargets(1, &RTH, false, &DSH);
	command_buffer.list->ClearRenderTargetView(RTV.handle.at(frame_buffer_index), kClearColor, 0, nullptr);
	command_buffer.list->ClearDepthStencilView(DSV.handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void D3D12Manager::SetViewport()
{
	command_buffer.list->RSSetViewports(1, &viewport);
	command_buffer.list->RSSetScissorRects(1, &scissor_rect);
}

void D3D12Manager::WaitFence()
{
	auto fence = sync.fences.at(frame_buffer_index);
	auto value = ++sync.fence_value.at(frame_buffer_index); // 現在のフェンスに GPU が到達後設定される値をセット

	command_buffer.queue->Signal(fence.Get(), value);

	// 次フレームで処理するコマンドの実行完了を待機する
	auto next = swap_chain4->GetCurrentBackBufferIndex();
	auto finish = sync.fence_value.at(next);
	fence = sync.fences.at(next);
	value = fence->GetCompletedValue();
	if (value < finish)
	{
		fence->SetEventOnCompletion(finish, sync.fence_event);
		WaitForSingleObject(sync.fence_event, INFINITE);
	}
}

void D3D12Manager::WaitIdle()
{
	constexpr UINT64 expect_value = 1;
	command_buffer.queue->Signal(sync.fence.Get(), expect_value);

	if (sync.fence->GetCompletedValue() != expect_value)
	{
		sync.fence->SetEventOnCompletion(expect_value, sync.fence_event);
		WaitForSingleObject(sync.fence_event, INFINITE);
	}
}

void D3D12Manager::CreateDebugLayer()
{
	ComPtr<ID3D12Debug> debug_layer;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_layer))))
	{
		debug_layer->EnableDebugLayer();
	}
}

void D3D12Manager::CreateFactory(UINT _dxgiflags)
{
	ComPtr<IDXGIFactory2> factory2;
	ASSERT_SUCCEEDED(CreateDXGIFactory2(_dxgiflags, IID_PPV_ARGS(&factory2)));
	factory2.As(&factory6);
}

void D3D12Manager::CreateDevice(UINT _dxgiflags)
{
	bool adapter_not_found = false;
	ComPtr<IDXGIAdapter4> adapter4;
	DXGI_ADAPTER_DESC3 adapter_desc{};
	for (UINT i = 0; ; ++i)
	{
		// 指定したGPUプリファレンスに基づいてグラフィックスアダプタを列挙する
		if (factory6->EnumAdapterByGpuPreference(1, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter4)) == DXGI_ERROR_NOT_FOUND)
		{
			adapter_not_found = true;
			break;
		}

		// デバイスの情報取得
		adapter4->GetDesc3(&adapter_desc);

		// ハードウェアアダプターでサポートされているものを探す
		if (!(adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE))
		{
			break;
		}
	}

	// 作成可能なアダプターがない場合
	if (adapter_not_found)
	{
		ASSERT_SUCCEEDED(factory6->EnumWarpAdapter(IID_PPV_ARGS(&adapter4)));
	}

	ComPtr<ID3D12Device> device;
	ASSERT_SUCCEEDED(D3D12CreateDevice(adapter4.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
	device.As(&device5);
}

void D3D12Manager::CreateInfoQueue()
{
	ComPtr<ID3D12InfoQueue> info_queue;
	if (SUCCEEDED(device5->QueryInterface(IID_PPV_ARGS(&info_queue))))
	{
		D3D12_MESSAGE_SEVERITY seviritys[] =
		{
			D3D12_MESSAGE_SEVERITY_INFO
		};

		D3D12_MESSAGE_ID denyIDs[] =
		{
			D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
			D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
		};

		D3D12_INFO_QUEUE_FILTER filter =
		{
			.DenyList =
			{
				.NumSeverities = _countof(seviritys),
				.pSeverityList = seviritys,
				.NumIDs = _countof(denyIDs),
				.pIDList = denyIDs
			}
		};

		info_queue->PushStorageFilter(&filter);
	}
}

void D3D12Manager::CreateCommandBuffer()
{
	constexpr D3D12_COMMAND_QUEUE_DESC command_queue_desc =
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT, 				// 直接コマンドリスト、GPUの状態を継承しない
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,	// 指定なし
		.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE, 				// GPUタイムアウト有効
		.NodeMask = 0 													// 単一アダプタのため0
	};

	ASSERT_SUCCEEDED(device5->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&command_buffer.queue)));

	for (UINT i = 0; i < kFrameBufferCount; ++i)
	{
		ASSERT_SUCCEEDED(device5->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_buffer.allocators.at(i))));
	}
	
	ASSERT_SUCCEEDED(device5->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_buffer.allocators.at(0).Get(), nullptr, IID_PPV_ARGS(&command_buffer.list)));
	command_buffer.list->Close();
}

void D3D12Manager::CreateSwapChain(HWND _hwnd)
{
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc =
	{
		.Width = kWindowWidth,
		.Height = kWindowHeight,
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.Stereo = false,											// フルスクリーン表示モードかバックバッファをステレオにするかどうかを指定する
		.SampleDesc =
		{
			.Count = 1,
			.Quality = 0
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = kFrameBufferCount,
		.Scaling = DXGI_SCALING_STRETCH,						// バックバッファの内容をプレゼンテーションのターゲットサイズに合わせる
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,		// Present1を呼び出した後、バックバッファの内容を破棄するよう指定する
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,			// 透過動作を指定しない
		.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH // ウィンドウモードとフルスクリーンモードの切り替えが可能
	};

	ComPtr<IDXGISwapChain1> swap_chain1;
	ASSERT_SUCCEEDED(factory6->CreateSwapChainForHwnd(command_buffer.queue.Get(), _hwnd, &swap_chain_desc, nullptr, nullptr, &swap_chain1));
	swap_chain1.As(&swap_chain4);

	// ALT+ENTER禁止
	factory6->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER);
}

void D3D12Manager::CreateDescriptorHeap()
{
	constexpr D3D12_DESCRIPTOR_HEAP_DESC RTV = 
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,	// レンダーターゲットビュー作成用
		.NumDescriptors = kMaxDescriptorCountRTV, // ディスクリプタの数
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE, // デフォルトの使用法を示す
		.NodeMask = 0										// 単一アダプタのため0
	};
	descriptor_heap.RTV = std::make_shared<DescriptorManager>(device5.Get(), RTV);

	constexpr D3D12_DESCRIPTOR_HEAP_DESC DSV =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,	// デプスステンシルビュー作成用
		.NumDescriptors = kMaxDescriptorCountDSV, // ディスクリプタの数
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE, // デフォルトの使用法を示す
		.NodeMask = 0										// 単一アダプタのため0
	};
	descriptor_heap.DSV = std::make_shared<DescriptorManager>(device5.Get(), DSV);

	constexpr D3D12_DESCRIPTOR_HEAP_DESC CBV_SRV_UAV =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,		// デプスステンシルビュー作成用
		.NumDescriptors = kMaxDescriptorCountDSV,				// ディスクリプタの数
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,// シェーダーで使用する
		.NodeMask = 0													// 単一アダプタのため0
	};
	descriptor_heap.CBV_SRV_UAV = std::make_shared<DescriptorManager>(device5.Get(), CBV_SRV_UAV);
}

void D3D12Manager::CreateRenderTargetView()
{
	for (UINT i = 0; i < kFrameBufferCount; ++i)
	{
		ASSERT_SUCCEEDED(swap_chain4->GetBuffer(i, IID_PPV_ARGS(&RTV.buffer.at(i))));

		RTV.handle.at(i) = descriptor_heap.RTV->Allocate();

		device5->CreateRenderTargetView(RTV.buffer.at(i).Get(), nullptr, RTV.handle.at(i));
	}
}

void D3D12Manager::CreateDepthStencilView()
{
	constexpr D3D12_HEAP_PROPERTIES heap_properties =
	{
		.Type = D3D12_HEAP_TYPE_DEFAULT,							// CPUアクセスを提供できない、GPUはプールからメモリへの読み書きが可能
		.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,// ヒープのCPUページプロパティは不明
		.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,	// ヒープのメモリプールは不明
		.CreationNodeMask = 1,										// 0,1どちらでも同じ
		.VisibleNodeMask = 1											// CreationNodeMaskと同じ値である必要がある
	};

	constexpr D3D12_RESOURCE_DESC resource_desc =
	{
		.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,	// Texture2Dとして使用する
		.Alignment = 0,												// アライメントの指定
		.Width = kWindowWidth,										// 画面横幅
		.Height = kWindowHeight,									// 画面縦幅
		.DepthOrArraySize = 1,										// 配列サイズの指定
		.MipLevels = 0,												// ミップレベルの数の指定
		.Format = DXGI_FORMAT_D32_FLOAT,							// フォーマットの指定
		.SampleDesc = 
		{
			.Count = 1,													// 画素あたりのマルチサンプル数
			.Quality = 0												// 画質レベル、画質が高いほどパフォーマンスが低下する
		},
		.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,				// 不明
		.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL	// デプスステンシルビューが作成できるように指定
	};

	constexpr D3D12_CLEAR_VALUE clear_value =
	{
		.Format = DXGI_FORMAT_D32_FLOAT, // リソースと合わせる
		.DepthStencil = 
		{
			.Depth = 1.0f,						// 深度値
			.Stencil = 0						// ステンシル値
		}
	};

	ASSERT_SUCCEEDED(device5->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear_value, IID_PPV_ARGS(&DSV.buffer)));

	constexpr D3D12_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc =
	{
		.Format = DXGI_FORMAT_D32_FLOAT,						// リソースと合わせる
		.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D, // デプスステンシルリソースがどのようにアクセスされるかを指定する
		.Flags = D3D12_DSV_FLAG_NONE,							// 読み取り専用ではないことを指定する
		.Texture2D = 
		{
			.MipSlice = 0											// 最初に使用するミップマップレベルのインデックス
		}
	};

	DSV.handle = descriptor_heap.DSV->Allocate();
	device5->CreateDepthStencilView(DSV.buffer.Get(), &depth_stencil_view_desc, DSV.handle);
}

void D3D12Manager::CreateFence()
{
	for (UINT i = 0; i < kFrameBufferCount; ++i)
	{
		ASSERT_SUCCEEDED(device5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&sync.fences.at(i))));
		sync.fence_value.at(i) = 0;
	}

	ASSERT_SUCCEEDED(device5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&sync.fence)));

	sync.fence_event = CreateEvent(nullptr, false, false, nullptr);
}

void D3D12Manager::SetupViewport()
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(kWindowWidth);
	viewport.Height = static_cast<float>(kWindowHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	scissor_rect.left = 0;
	scissor_rect.top = 0;
	scissor_rect.right = static_cast<LONG>(kWindowWidth);
	scissor_rect.bottom = static_cast<LONG>(kWindowHeight);
}