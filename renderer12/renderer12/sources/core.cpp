#include "core.h"
#include "constants.h"
#include "utility.h"

using namespace Microsoft::WRL;

void D3D12Core::Initialize(HWND hwnd)
{
	HRESULT hr = S_OK;

	UINT dxgi_flags = 0u;
#if _DEBUG
	ComPtr<ID3D12Debug> debug_layer;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_layer))))
	{
		debug_layer->EnableDebugLayer();
		dxgi_flags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	ComPtr<IDXGIFactory4> dxgi_factory;
	hr = CreateDXGIFactory2(dxgi_flags, IID_PPV_ARGS(&dxgi_factory));
	ASSERT(SUCCEEDED(hr), utility::HRTrace(hr));

	constexpr D3D_FEATURE_LEVEL kFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	ComPtr<IDXGIAdapter1> adapter;
	for (UINT i = 0; dxgi_factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		// ハードウェアアダプターでサポートされているものを探す
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		if (SUCCEEDED(D3D12CreateDevice(
			adapter.Get(),
			kFeatureLevel,
			__uuidof(ID3D12Device),
			nullptr)))
			break;
	}

	hr = D3D12CreateDevice(adapter.Get(), kFeatureLevel, IID_PPV_ARGS(&device_));
	ASSERT(SUCCEEDED(hr), utility::HRTrace(hr));

	D3D12_COMMAND_QUEUE_DESC queue_desc{
	D3D12_COMMAND_LIST_TYPE_DIRECT,			// コマンドリストと合わせる
	D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,	// プライオリティは特に指定なし
	D3D12_COMMAND_QUEUE_FLAG_NONE,			// タイムアウト無し
	0													// アダプターが１つなら０でよし
	};
	hr = device_->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_queue_));
	ASSERT(SUCCEEDED(hr), utility::HRTrace(hr));

	hr = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&command_allocator_));
	ASSERT(SUCCEEDED(hr), utility::HRTrace(hr));

	hr = device_->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		command_allocator_.Get(),
		nullptr,
		IID_PPV_ARGS(&graphics_command_list_));
	ASSERT(SUCCEEDED(hr), utility::HRTrace(hr));
	graphics_command_list_->Close();

	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
	swap_chain_desc.Width = constants::kResolution.x;
	swap_chain_desc.Height = constants::kResolution.y;
	swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.Stereo = false;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swap_chain_desc.BufferCount = RenderTarget::kFrameCount;
	swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;						 // バックバッファーは伸び縮み可能
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		 // フリップ後は速やかに破棄
	swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;			 // 特に指定なし
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ウィンドウ、フルスクリーン切り替え可能尾

	ComPtr<IDXGISwapChain1> swap_chain;
	hr = dxgi_factory->CreateSwapChainForHwnd(
		command_queue_.Get(),
		hwnd,
		&swap_chain_desc,
		nullptr,
		nullptr,
		&swap_chain);
	ASSERT(SUCCEEDED(hr), utility::HRTrace(hr));
	swap_chain.As(&swap_chain_);

	D3D12_DESCRIPTOR_HEAP_DESC render_target_descriptor_heap_desc{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,	// 作成するビューの指定
		RenderTarget::kFrameCount,			// 表裏の2つ
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,	// 特になし
		0											// 複数のGPUがある場合に識別するビットフラグ 
	};
	hr = device_->CreateDescriptorHeap(
		&render_target_descriptor_heap_desc,
		IID_PPV_ARGS(&render_target_.descriptor_heap_));

	D3D12_RENDER_TARGET_VIEW_DESC render_target_desc{
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D12_RTV_DIMENSION_TEXTURE2D
	};
	CD3DX12_CPU_DESCRIPTOR_HANDLE render_target_cpu_handle(
		render_target_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());
	render_target_.descriptor_size_ = 
		device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	render_target_.resource_.resize(RenderTarget::kFrameCount);
	for (UINT i = 0; i < RenderTarget::kFrameCount; ++i)
	{
		hr = swap_chain_->GetBuffer(
			i, IID_PPV_ARGS(&render_target_.resource_[i]));
		ASSERT(SUCCEEDED(hr), utility::HRTrace(hr));

		device_->CreateRenderTargetView(
			render_target_.resource_[i].Get(),
			&render_target_desc,
			render_target_cpu_handle);

		render_target_cpu_handle.Offset(1, render_target_.descriptor_size_);
	}

	D3D12_DESCRIPTOR_HEAP_DESC depth_stencil_descriptor_heap_desc{
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,	// 作成するビューの指定
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,	// 特になし
		0											// 複数のGPUがある場合に識別するビットフラグ 
	};
	hr = device_->CreateDescriptorHeap(
		&depth_stencil_descriptor_heap_desc,
		IID_PPV_ARGS(&depth_stencil_.descriptor_heap_));

	CD3DX12_RESOURCE_DESC depth_buffer_resource_desc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			constants::kResolution.x,
			constants::kResolution.y,
			1, 0, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	D3D12_CLEAR_VALUE depth_clear_value = {};
	depth_clear_value.Format = depth_buffer_resource_desc.Format;
	depth_clear_value.DepthStencil.Depth = 1.0f;
	depth_clear_value.DepthStencil.Stencil = 0;

	hr = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depth_buffer_resource_desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depth_clear_value,
		IID_PPV_ARGS(&depth_stencil_.resource_));

	D3D12_DEPTH_STENCIL_VIEW_DESC depth_stencil_desc{
	DXGI_FORMAT_D32_FLOAT,
	D3D12_DSV_DIMENSION_TEXTURE2D,
	D3D12_DSV_FLAG_NONE,
		{
			0
		}
	};
	CD3DX12_CPU_DESCRIPTOR_HANDLE depth_stencil_cpu_handle(
		depth_stencil_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());
	device_->CreateDepthStencilView(
		depth_stencil_.resource_.Get(),
		&depth_stencil_desc,
		depth_stencil_cpu_handle);

	hr = device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	ASSERT(SUCCEEDED(hr), utility::HRTrace(hr));

	hfence_ = CreateEvent(nullptr, false, false, nullptr);
	fence_value_ = 0;

	viewport_ = CD3DX12_VIEWPORT(
		0.0f,
		0.0f,
		static_cast<float>(constants::kResolution.x),
		static_cast<float>(constants::kResolution.y));

	scissor_ = CD3DX12_RECT(
		0,
		0,
		static_cast<LONG>(constants::kResolution.x),
		static_cast<LONG>(constants::kResolution.y));
}

void D3D12Core::UnInitialize()
{
	WaitGPU();
}

void D3D12Core::Reset()
{
	command_allocator_->Reset();
	graphics_command_list_->Reset(command_allocator_.Get(), nullptr);

	frame_index_ = swap_chain_->GetCurrentBackBufferIndex();
}

void D3D12Core::Barrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		render_target_.resource_[frame_index_].Get(),
		before,
		after
	);
	graphics_command_list_->ResourceBarrier(1, &barrier);
}

void D3D12Core::ClearScreen()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE render_target_view(
		render_target_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart(),
		frame_index_,
		render_target_.descriptor_size_);
	CD3DX12_CPU_DESCRIPTOR_HANDLE depth_stencil_view(
		depth_stencil_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());
	graphics_command_list_->OMSetRenderTargets(
		1,
		&render_target_view,
		true,
		&depth_stencil_view);

	constexpr float clear_color[] = { 0,0,0,1 };
	graphics_command_list_->ClearRenderTargetView(
		render_target_view,
		clear_color,
		0,
		nullptr);
	graphics_command_list_->ClearDepthStencilView(
		depth_stencil_view,
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr);
}

void D3D12Core::Present()
{
	graphics_command_list_->Close();

	ID3D12CommandList* lists[] = { graphics_command_list_.Get() };
	command_queue_->ExecuteCommandLists(1, lists);

	swap_chain_->Present(1, 0);
}

void D3D12Core::WaitGPU()
{
	command_queue_->Signal(fence_.Get(), ++fence_value_);
	if (fence_->GetCompletedValue() != fence_value_)
	{
		fence_->SetEventOnCompletion(fence_value_, hfence_);
		WaitForSingleObject(hfence_, INFINITE);
	}
}