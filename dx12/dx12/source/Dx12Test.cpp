#include "../header/Dx12Test.h"
#include "../header/utility.h"

using namespace Microsoft::WRL;

void Dx12Test::Initialize(HWND _hwnd)
{
	UINT dxgiflags = 0;
#if _DEBUG
	dxgiflags |= DXGI_CREATE_FACTORY_DEBUG;
	ComPtr<ID3D12Debug> debug_layer;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_layer))))
	{
		// デバックレイヤーの有効化
		debug_layer->EnableDebugLayer();
	}
#endif
	ComPtr<IDXGIFactory2> factory2;
	ASSERT_SUCCEEDED(CreateDXGIFactory2(dxgiflags, IID_PPV_ARGS(&factory2)));
	factory2.As(&factory6);

	ComPtr<IDXGIAdapter1> adapter;
	DXGI_ADAPTER_DESC1 adapter_desc{};
	for (UINT i = 0; ; ++i)
	{
		if (factory6->EnumAdapters1(1, &adapter) != DXGI_ERROR_NOT_FOUND)
		{
			adapter->GetDesc1(&adapter_desc);
		}
		// ハードウェアでない場合はもう一度
		if (adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}
		// 作成可能か
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	// デバイスの作成
	ComPtr<ID3D12Device> device;
	ASSERT_SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
	device.As(&device5);

	// コマンドキューの作成
	constexpr D3D12_COMMAND_QUEUE_DESC command_queue_desc =
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT, 				// 直接コマンドリスト、GPUの状態を継承しない
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,	// 指定なし
		.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE, 				// GPUタイムアウト有効
		.NodeMask = 0 													// 単一アダプタのため0
	};
	ASSERT_SUCCEEDED(device5->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&command_queue)));

	// コマンドアロケーターの生成
	ASSERT_SUCCEEDED(device5->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator)));
	
	// コマンドリストの作成
	ASSERT_SUCCEEDED(device5->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator.Get(), nullptr, IID_PPV_ARGS(&command_list)));
	command_list->Close();

	// スワップチェインの作成
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
		//.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH // ウィンドウモードとフルスクリーンモードの切り替えが可能
	};

	ComPtr<IDXGISwapChain1> swap_chain1;
	ASSERT_SUCCEEDED(factory6->CreateSwapChainForHwnd(command_queue.Get(), _hwnd, &swap_chain_desc, nullptr, nullptr, &swap_chain1));
	swap_chain1.As(&swap_chain4);

	// ALT+ENTER禁止
	factory6->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER);

	// ディスクリプタヒープ(RTV)の作成
	constexpr D3D12_DESCRIPTOR_HEAP_DESC RTV_heap_desc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,	// レンダーターゲットビュー作成用
		.NumDescriptors = kFrameBufferCount,		// ディスクリプタの数
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE, // デフォルトの使用法を示す
		.NodeMask = 0										// 単一アダプタのため0
	};
	ASSERT_SUCCEEDED(device5->CreateDescriptorHeap(&RTV_heap_desc, IID_PPV_ARGS(&RTV_heap)));
	RTV_heap_size = device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// レンダーターゲットビューの作成
	D3D12_CPU_DESCRIPTOR_HANDLE RTV_handle = RTV_heap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < kFrameBufferCount; ++i)
	{
		ASSERT_SUCCEEDED(swap_chain4->GetBuffer(i, IID_PPV_ARGS(&back_buffer.at(i))));

		device5->CreateRenderTargetView(back_buffer.at(i).Get(), nullptr, RTV_handle);
		RTV_handle.ptr += RTV_heap_size;
	}

	// ディスクリプタヒープ(DSV)の作成
	constexpr D3D12_DESCRIPTOR_HEAP_DESC DSV_heap_desc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,	// レンダーターゲットビュー作成用
		.NumDescriptors = 1,								// ディスクリプタの数
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE, // デフォルトの使用法を示す
		.NodeMask = 0										// 単一アダプタのため0
	};
	ASSERT_SUCCEEDED(device5->CreateDescriptorHeap(&DSV_heap_desc, IID_PPV_ARGS(&DSV_heap)));

	// デプスバッファの作成
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
	ASSERT_SUCCEEDED(device5->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear_value, IID_PPV_ARGS(&depth_buffer)));

	// デプスステンシルビューの作成
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
	device5->CreateDepthStencilView(depth_buffer.Get(), &depth_stencil_view_desc, DSV_heap->GetCPUDescriptorHandleForHeapStart());

	// フェンスの作成
	ASSERT_SUCCEEDED(device5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	fence_event = CreateEvent(nullptr, false, false, nullptr);
	fence_value = 0;

	// ビューポートの設定
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

void Dx12Test::RenderBegin()
{
	// 現在のバックバッファのインデックス取得
	frame_buffer_index = swap_chain4->GetCurrentBackBufferIndex();

	// コマンドのリセット
	command_allocator->Reset();
	command_list->Reset(command_allocator.Get(), nullptr);

	// リソースバリアの設定
	const D3D12_RESOURCE_BARRIER barrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = back_buffer.at(frame_buffer_index).Get(),
			.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			.StateBefore = D3D12_RESOURCE_STATE_PRESENT,
			.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET
		}
	};
	command_list->ResourceBarrier(1, &barrier);

	// 画面のクリア
	D3D12_CPU_DESCRIPTOR_HANDLE RTV_handle = RTV_heap->GetCPUDescriptorHandleForHeapStart();
	RTV_handle.ptr += static_cast<size_t>(frame_buffer_index) * static_cast<size_t>(RTV_heap_size);
	D3D12_CPU_DESCRIPTOR_HANDLE DSV_handle = DSV_heap->GetCPUDescriptorHandleForHeapStart();
	command_list->OMSetRenderTargets(1, &RTV_handle, false, &DSV_handle);
	command_list->ClearRenderTargetView(RTV_handle, kClearColor, 0, nullptr);
	command_list->ClearDepthStencilView(DSV_handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポートのセット
	command_list->RSSetViewports(1, &viewport);
	command_list->RSSetScissorRects(1, &scissor_rect);
}

void Dx12Test::RenderEnd()
{
	// リソースバリアの設定
	const D3D12_RESOURCE_BARRIER barrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = back_buffer.at(frame_buffer_index).Get(),
			.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
			.StateAfter = D3D12_RESOURCE_STATE_PRESENT
		}
	};
	command_list->ResourceBarrier(1, &barrier);

	// コマンドの積み込み終了
	command_list->Close();

	// コマンドリストの実行
	ID3D12CommandList* execute_list[] = { command_list.Get() };
	command_queue->ExecuteCommandLists(1, execute_list);

	swap_chain4->Present(0, 0);

	// GPUの処理が完了したか確認する
	WaitFence();
}

void Dx12Test::WaitFence()
{
	command_queue->Signal(fence.Get(), ++fence_value);
	if (fence->GetCompletedValue() < fence_value)
	{
		fence->SetEventOnCompletion(fence_value, fence_event);
		WaitForSingleObject(fence_event, INFINITE);
	}
}