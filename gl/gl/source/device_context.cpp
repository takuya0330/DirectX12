#include "device_context.h"
#include "administrator.h"
#include "device.h"
#include "assertion.h"

namespace gl
{
	device_context::device_context(HWND _hwnd)
	{
		ID3D12Device5* d3d12device = administrator::get<device>()->get_device();
		IDXGIFactory6* factory = administrator::get<device>()->get_factory();

		create_command_queue(d3d12device, D3D12_COMMAND_LIST_TYPE_DIRECT);

		create_command_allocator(d3d12device, D3D12_COMMAND_LIST_TYPE_DIRECT);

		create_command_list(d3d12device, D3D12_COMMAND_LIST_TYPE_DIRECT);

		create_swap_chain(_hwnd, factory);

		create_render_target(d3d12device);

		create_depth_stencil(d3d12device);

		create_fence(d3d12device);
	}

	void device_context::reset()
	{
		frame_buffer_index_ = swap_chain_->GetCurrentBackBufferIndex();

		command_allocator_->Reset();
		command_list_->Reset(command_allocator_.Get(), nullptr);
	}

	void device_context::barrier_transition(D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _affter)
	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(render_target_.buffers_[frame_buffer_index_].Get(), _before, _affter);
		command_list_->ResourceBarrier(1, &barrier);
	}

	void device_context::clear(const float* _clear_color)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE render_target_view(render_target_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), frame_buffer_index_, render_target_.increment_size_);
		CD3DX12_CPU_DESCRIPTOR_HANDLE depth_stencil_view(depth_stencil_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());
		command_list_->OMSetRenderTargets(1, &render_target_view, true, &depth_stencil_view);
		command_list_->ClearRenderTargetView(render_target_view, _clear_color, 0, nullptr);
		command_list_->ClearDepthStencilView(depth_stencil_view, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}

	void device_context::set_viewport(const uint2& _window_size)
	{
		viewport_ = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(_window_size.x), static_cast<float>(_window_size.y));
		scissor_rect_ = CD3DX12_RECT(0, 0, static_cast<LONG>(_window_size.x), static_cast<LONG>(_window_size.y));
		command_list_->RSSetViewports(1, &viewport_);
		command_list_->RSSetScissorRects(1, &scissor_rect_);
	}

	void device_context::present(uint _sync)
	{
		command_list_->Close();

		ID3D12CommandList* list = command_list_.Get();
		command_queue_->ExecuteCommandLists(1, &list);

		swap_chain_->Present(_sync, 0);
	}

	void device_context::wait_previous_frame()
	{
		command_queue_->Signal(sync_.fence_.Get(), ++sync_.value_);
		if (sync_.fence_->GetCompletedValue() != sync_.value_)
		{
			sync_.fence_->SetEventOnCompletion(sync_.value_, sync_.event_);
			WaitForSingleObject(sync_.event_, INFINITE);
		}
	}

	void device_context::create_swap_chain(HWND _hwnd, IDXGIFactory6* _factory)
	{
		DXGI_SWAP_CHAIN_DESC1 desc = {};
		desc.Width = kWindowSize.x;
		desc.Height = kWindowSize.y;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Stereo = false;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = kFrameBufferNum;
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ComPtr<IDXGISwapChain1> swap_chain1;
		_ASSERT_EXPR_A(SUCCEEDED(_factory->CreateSwapChainForHwnd(command_queue_.Get(), _hwnd, &desc, nullptr, nullptr, &swap_chain1)), "CreateSwapChainForHwnd is failed.");

		swap_chain1.As(&swap_chain_);
	}

	void device_context::create_command_queue(ID3D12Device* _device, D3D12_COMMAND_LIST_TYPE _command_list_type)
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = _command_list_type;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		_ASSERT_EXPR_A(SUCCEEDED(_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&command_queue_))), "CreateCommandQueue is failed.");
	}

	void device_context::create_command_allocator(ID3D12Device* _device, D3D12_COMMAND_LIST_TYPE _command_list_type)
	{
		_ASSERT_EXPR_A(SUCCEEDED(_device->CreateCommandAllocator(_command_list_type, IID_PPV_ARGS(&command_allocator_))), "CreateCommandAllocator is failed.");
	}

	void device_context::create_command_list(ID3D12Device* _device, D3D12_COMMAND_LIST_TYPE _command_list_type)
	{
		_ASSERT_EXPR_A(SUCCEEDED(_device->CreateCommandList(0, _command_list_type, command_allocator_.Get(), nullptr, IID_PPV_ARGS(&command_list_))), "CreateCommandList is failed.");
		command_list_->Close();
	}

	void device_context::create_render_target(ID3D12Device* _device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc = {};
		descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descriptor_heap_desc.NumDescriptors = kFrameBufferNum;
		descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descriptor_heap_desc.NodeMask = 0;

		_ASSERT_EXPR_A(SUCCEEDED(_device->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(&render_target_.descriptor_heap_))), "CreateDescriptorHeap is failed.");
	
		render_target_.increment_size_ = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(render_target_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());

		D3D12_RENDER_TARGET_VIEW_DESC render_target_view_desc = {};
		render_target_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		render_target_view_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		for (uint i = 0; i < kFrameBufferNum; ++i)
		{
			_ASSERT_EXPR_A(SUCCEEDED(swap_chain_->GetBuffer(i, IID_PPV_ARGS(&render_target_.buffers_[i]))), "GetBuffer is failed.");
			_device->CreateRenderTargetView(render_target_.buffers_[i].Get(), &render_target_view_desc, cpu_handle);
			cpu_handle.Offset(render_target_.increment_size_);
		}
	}

	void device_context::create_depth_stencil(ID3D12Device* _device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc = {};
		descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		descriptor_heap_desc.NumDescriptors = 1;
		descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descriptor_heap_desc.NodeMask = 0;

		_ASSERT_EXPR_A(SUCCEEDED(_device->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(&depth_stencil_.descriptor_heap_))), "CreateDescriptorHeap is failed.");

		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(depth_stencil_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());

		D3D12_RESOURCE_DESC resource_desc = {};
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resource_desc.Format = DXGI_FORMAT_D32_FLOAT;
		resource_desc.Width = kWindowSize.x;
		resource_desc.Height = kWindowSize.y;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.SampleDesc = { 1,0 };
		resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_HEAP_PROPERTIES heap_properties = {};
		heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_CLEAR_VALUE depth_clear_value = {};
		depth_clear_value.Format = DXGI_FORMAT_D32_FLOAT;
		depth_clear_value.DepthStencil.Depth = 1.0f;
		depth_clear_value.DepthStencil.Stencil = 0;

		_ASSERT_EXPR_A(SUCCEEDED(_device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depth_clear_value, IID_PPV_ARGS(&depth_stencil_.buffer_))), "CreateCommittedResource is failed.");

		D3D12_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc = {};
		depth_stencil_view_desc.Format = DXGI_FORMAT_D32_FLOAT;
		depth_stencil_view_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		depth_stencil_view_desc.Flags = D3D12_DSV_FLAG_NONE;
		depth_stencil_view_desc.Texture2D.MipSlice = 0;

		_device->CreateDepthStencilView(depth_stencil_.buffer_.Get(), &depth_stencil_view_desc, cpu_handle);
	}

	void device_context::create_fence(ID3D12Device* _device)
	{
		_ASSERT_EXPR_A(SUCCEEDED(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&sync_.fence_))), "CreateFence is failed.");
		sync_.event_ = CreateEvent(nullptr, false, false, nullptr);
		sync_.value_ = 0;
	}
}