#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

#include "../include/d3d12_manager.h"
#include "../include/utility.h"

bool D3D12Manager::Initialize(HWND _hwnd, UINT _width, UINT _height)
{
	HRESULT hr = S_OK;
	UINT dxgiflags = 0;
#if _DEBUG
	dxgiflags |= DXGI_CREATE_FACTORY_DEBUG;

	// デバッグレイヤーの有効化
	ComPtr<ID3D12Debug> debug;
	hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	if (SUCCEEDED(hr))
	{
		debug->EnableDebugLayer();
	}
	else
	{
		Debug::Console("D3D12GetDebugInterface failed.\n");
		return false;
	}
#endif

	// ファクトリーの作成
	ComPtr<IDXGIFactory2> factory2;
	hr = CreateDXGIFactory2(dxgiflags, IID_PPV_ARGS(&factory2));
	if (FAILED(hr))
	{
		Debug::Console("CreateDXGIFactory2 failed.\n");
		return false;
	}
	factory2.As(&factory_);

	// アダプターの検索
	bool adapter_not_found = false;
	ComPtr<IDXGIAdapter4> adapter4;
	DXGI_ADAPTER_DESC3 adapter_desc{};
	for (UINT i = 0; ; ++i)
	{
		// 指定したGPUプリファレンスに基づいてグラフィックスアダプタを列挙する
		if (factory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter4)) == DXGI_ERROR_NOT_FOUND)
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
		hr = factory_->EnumWarpAdapter(IID_PPV_ARGS(&adapter4));
		if (FAILED(hr))
		{
			Debug::Console("EnumWarpAdapter failed.\n");
			return false;
		}
	}

	// デバイスの作成
	ComPtr<ID3D12Device> device;
	hr = D3D12CreateDevice(adapter4.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
	if (FAILED(hr))
	{
		Debug::Console("D3D12CreateDevice failed.\n");
		return false;
	}
	device.As(&device_);

#if _DEBUG
	// 
	ComPtr<ID3D12InfoQueue> info_queue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&info_queue))))
	{
		D3D12_MESSAGE_SEVERITY seviritys[] = {
			D3D12_MESSAGE_SEVERITY_INFO
		};
		D3D12_MESSAGE_ID denyIDs[] = {
			D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
			D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
		};
		D3D12_INFO_QUEUE_FILTER filter = {
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
#endif

	// コマンドキューの作成
	constexpr D3D12_COMMAND_QUEUE_DESC command_queue_desc = {
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT, 				// 直接コマンドリスト、GPUの状態を継承しない
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,	// 指定なし
		.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE, 				// GPUタイムアウト有効
		.NodeMask = 0 													// 単一アダプタのため0
	};
	hr = device_->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&command_queue_));
	if (FAILED(hr))
	{
		Debug::Console("CreateCommandQueue failed.\n");
		return false;
	}

	// コマンドアロケーターの作成
	for (UINT i = 0; i < kFrameBufferCount; ++i)
	{
		hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocators_.at(i)));
		if (FAILED(hr))
		{
			Debug::Console("CreateCommandAllocator failed.\n");
			return false;
		}
	}

	// コマンドリストの作成
	hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocators_.at(0).Get(), nullptr, IID_PPV_ARGS(&command_list_));
	if (FAILED(hr))
	{
		Debug::Console("CreateCommandList failed.\n");
		return false;
	}
	command_list_->Close();

	// スワップチェインの作成
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {
		.Width = _width,
		.Height = _height,
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
	hr = factory_->CreateSwapChainForHwnd(command_queue_.Get(), _hwnd, &swap_chain_desc, nullptr, nullptr, &swap_chain1);
	if (FAILED(hr))
	{
		Debug::Console("CreateSwapChainForHwnd failed.\n");
		return false;
	}
	swap_chain1.As(&swap_chain_);

	// ALT+ENTER禁止
	factory_->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER);

	// レンダーターゲットの作成
	constexpr D3D12_DESCRIPTOR_HEAP_DESC render_target_heap_desc = {
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,	// レンダーターゲットビュー作成用
		.NumDescriptors = 1,								// ディスクリプタの数
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE, // デフォルトの使用法を示す
		.NodeMask = 0										// 単一アダプタのため0
	};
	for (UINT i = 0; i < kFrameBufferCount; ++i)
	{
		hr = device_->CreateDescriptorHeap(&render_target_heap_desc, IID_PPV_ARGS(&render_target_heaps_.at(i)));
		if (FAILED(hr))
		{
			Debug::Console("CreateDescriptorHeap failed.\n");
			return false;
		}
		render_target_cpu_handles_.at(i) = render_target_heaps_.at(i)->GetCPUDescriptorHandleForHeapStart();

		hr = swap_chain_->GetBuffer(i, IID_PPV_ARGS(&frame_buffers_.at(i)));
		if (FAILED(hr))
		{
			Debug::Console("GetBuffer failed.\n");
			return false;
		}

		device_->CreateRenderTargetView(frame_buffers_.at(i).Get(), nullptr, render_target_cpu_handles_.at(i));
	}

	// デプスステンシルの作成
	constexpr D3D12_HEAP_PROPERTIES heap_properties = {
		.Type = D3D12_HEAP_TYPE_DEFAULT,							// CPUアクセスを提供できない、GPUはプールからメモリへの読み書きが可能
		.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,// ヒープのCPUページプロパティは不明
		.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,	// ヒープのメモリプールは不明
		.CreationNodeMask = 1,										// 0,1どちらでも同じ
		.VisibleNodeMask = 1											// CreationNodeMaskと同じ値である必要がある
	};
	D3D12_RESOURCE_DESC resource_desc = {
		.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,	// Texture2Dとして使用する
		.Alignment = 0,												// アライメントの指定
		.Width = _width,												// 画面横幅
		.Height = _height,											// 画面縦幅
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
	constexpr D3D12_CLEAR_VALUE clear_value = {
		.Format = DXGI_FORMAT_D32_FLOAT, // リソースと合わせる
		.DepthStencil =
		{
			.Depth = 1.0f,						// 深度値
			.Stencil = 0						// ステンシル値
		}
	};
	hr = device_->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear_value, IID_PPV_ARGS(&depth_buffer_));
	if (FAILED(hr))
	{
		Debug::Console("CreateCommittedResource failed.\n");
		return false;
	}

	constexpr D3D12_DESCRIPTOR_HEAP_DESC depth_stencil_heap_desc = {
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,	// レンダーターゲットビュー作成用
		.NumDescriptors = 1,								// ディスクリプタの数
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE, // デフォルトの使用法を示す
		.NodeMask = 0										// 単一アダプタのため0
	};
	hr = device_->CreateDescriptorHeap(&depth_stencil_heap_desc, IID_PPV_ARGS(&depth_stencil_heap_));
	if (FAILED(hr))
	{
		Debug::Console("CreateDescriptorHeap failed.\n");
		return false;
	}
	depth_stencil_cpu_handle_ = depth_stencil_heap_->GetCPUDescriptorHandleForHeapStart();

	constexpr D3D12_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc = {
		.Format = DXGI_FORMAT_D32_FLOAT,						// リソースと合わせる
		.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D, // デプスステンシルリソースがどのようにアクセスされるかを指定する
		.Flags = D3D12_DSV_FLAG_NONE,							// 読み取り専用ではないことを指定する
		.Texture2D =
		{
			.MipSlice = 0											// 最初に使用するミップマップレベルのインデックス
		}
	};
	device_->CreateDepthStencilView(depth_buffer_.Get(), &depth_stencil_view_desc, depth_stencil_cpu_handle_);

	// フェンスの作成
	for (UINT i = 0; i < kFrameBufferCount; ++i)
	{
		hr = device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fences_.at(i)));
		if (FAILED(hr))
		{
			Debug::Console("CreateFence failed.\n");
			return false;
		}
		fence_values_.at(i) = 0;
	}
	fence_event_ = CreateEvent(nullptr, false, false, nullptr);

	// ビューポートの設定
	viewport_.TopLeftX = 0.0f;
	viewport_.TopLeftY = 0.0f;
	viewport_.Width = static_cast<float>(_width);
	viewport_.Height = static_cast<float>(_height);
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	scissor_rect_.left = 0;
	scissor_rect_.top = 0;
	scissor_rect_.right = static_cast<LONG>(_width);
	scissor_rect_.bottom = static_cast<LONG>(_height);

	return true;
}

void D3D12Manager::SetFrameIndex()
{
	frame_index_ = swap_chain_->GetCurrentBackBufferIndex();
}

void D3D12Manager::Reset()
{
	command_allocators_.at(frame_index_)->Reset();
	command_list_->Reset(command_allocators_.at(frame_index_).Get(), nullptr);
}

void D3D12Manager::BarrierRenderTarget()
{
	D3D12_RESOURCE_BARRIER barrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = frame_buffers_.at(frame_index_).Get(),
			.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			.StateBefore = D3D12_RESOURCE_STATE_PRESENT,
			.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET
		}
	};
	command_list_->ResourceBarrier(1, &barrier);
}

void D3D12Manager::ClearRenderTargetView(const float* _color)
{
	command_list_->ClearRenderTargetView(render_target_cpu_handles_.at(frame_index_), _color, 0, nullptr);
}

void D3D12Manager::ClearDepthStencilView()
{
	command_list_->ClearDepthStencilView(depth_stencil_cpu_handle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void D3D12Manager::SetRenderTargets()
{
	command_list_->OMSetRenderTargets(1, &render_target_cpu_handles_.at(frame_index_), false, &depth_stencil_cpu_handle_);
}

void D3D12Manager::SetViewport(UINT _width, UINT _height)
{
	viewport_.Width = static_cast<float>(_width);
	viewport_.Height = static_cast<float>(_height);
	scissor_rect_.right = static_cast<LONG>(_width);
	scissor_rect_.bottom = static_cast<LONG>(_height);
	command_list_->RSSetViewports(1, &viewport_);
	command_list_->RSSetScissorRects(1, &scissor_rect_);
}

void D3D12Manager::BarrierPresent()
{
	D3D12_RESOURCE_BARRIER barrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = frame_buffers_.at(frame_index_).Get(),
			.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
			.StateAfter = D3D12_RESOURCE_STATE_PRESENT
		}
	};
	command_list_->ResourceBarrier(1, &barrier);
}

void D3D12Manager::ExecuteCommandQueue()
{
	command_list_->Close();
	ID3D12CommandList* execute_list[] = { command_list_.Get() };
	command_queue_->ExecuteCommandLists(_countof(execute_list), execute_list);
}

void D3D12Manager::Present(UINT _vsync)
{
	swap_chain_->Present(_vsync, 0);
}

void D3D12Manager::WaitPreviousFrame()
{
	auto fence = fences_.at(frame_index_).Get();
	auto value = ++fence_values_.at(frame_index_);

	// フェンスを指定した値に更新
	command_queue_->Signal(fence, value);

	// 次フレームで処理するコマンドの実行完了を待機
	auto next_index = swap_chain_->GetCurrentBackBufferIndex();
	auto next_value = fence_values_.at(next_index);
	fence = fences_.at(next_index).Get();
	value = fence->GetCompletedValue();
	if (value < next_value)
	{
		// 未完了のためイベントで待機
		fence->SetEventOnCompletion(next_value, fence_event_);
		WaitForSingleObject(fence_event_, INFINITE);
	}
}

ComPtr<ID3DBlob> D3D12Manager::D3DCompile(const std::wstring& _filename, const std::string& _shader_model)
{
	UINT compile_flag = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined _DEBUG | DEBUG
	compile_flag |= D3DCOMPILE_DEBUG;
#endif
	ComPtr<ID3DBlob> out;
	ComPtr<ID3DBlob> err;
	auto hr = D3DCompileFromFile(
		_filename.c_str(), 
		nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		"main", 
		_shader_model.c_str(), 
		compile_flag, 
		0, 
		out.ReleaseAndGetAddressOf(), 
		err.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		Debug::Console("D3DCompileFromFile failed.");
		return nullptr;
	}
	if (err)
	{
		Debug::Console(static_cast<char*>(err->GetBufferPointer()));
		return nullptr;
	}
	return out;
}

// ---------------------------------------------------------

bool WriteUploadHeapMemory(ID3D12Resource* _resource, const void* _data, uint32_t _size)
{
	void* mapped = nullptr;
	auto hr = _resource->Map(0, nullptr, &mapped);
	if (FAILED(hr))
	{
		Debug::Console("Map failed.");
		return false;
	}
	memcpy(mapped, _data, _size);
	_resource->Unmap(0, nullptr);

	return true;
}