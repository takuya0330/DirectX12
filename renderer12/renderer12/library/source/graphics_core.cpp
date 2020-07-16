#include "../external/d3dx12/d3dx12.h"

#include "graphics_core.h"
//#include "buffer.h"
//#include "pipeline_state_object.h"
#include "utility.h"

using namespace Microsoft::WRL;
namespace snd
{
	void D3D12Core::Initialize(HWND _hwnd)
	{
#if _DEBUG
		constexpr uint dxgi_flags = DXGI_CREATE_FACTORY_DEBUG;
		CreateDebugLayer();
#else
		constexpr uint dxgi_flags = 0;
#endif

		ComPtr<IDXGIFactory4> factory = CreateFactory(dxgi_flags);

		ComPtr<IDXGIAdapter1> adapter = CreateAdapter(factory.Get(), D3D_FEATURE_LEVEL_11_0);

		CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0);

#if _DEBUG
		CreateInfoQueue();
#endif

		command_device_.queue_ = CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);

		command_device_.allocator_ = CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT);

		command_device_.list_ = CreateGraphicsCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT, command_device_.allocator_.Get());
		command_device_.list_->Close();

		CreateSwapChain(factory.Get(), _hwnd);

		CreateRenderTarget();

		CreateDepthStencil();

		CreateFence();
	}

	void D3D12Core::Reset()
	{
		current_back_buffer_index_ = swap_chain_->GetCurrentBackBufferIndex();
		command_device_.allocator_->Reset();
		command_device_.list_->Reset(command_device_.allocator_.Get(), nullptr);
	}

	void D3D12Core::Barrier(D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _affter)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = render_target_.buffers_[current_back_buffer_index_].Get();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = _before;
		barrier.Transition.StateAfter = _affter;
		command_device_.list_->ResourceBarrier(1, &barrier);
	}

	void D3D12Core::Clear(const float* _clear_color)
	{
		const CD3DX12_CPU_DESCRIPTOR_HANDLE render_target_view(render_target_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), current_back_buffer_index_, render_target_.increment_size_);
		const CD3DX12_CPU_DESCRIPTOR_HANDLE depth_stencil_view(depth_stencil_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());

		command_device_.list_->ClearRenderTargetView(render_target_view, _clear_color, 0, nullptr);
		command_device_.list_->ClearDepthStencilView(depth_stencil_view, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		command_device_.list_->OMSetRenderTargets(1, &render_target_view, false, &depth_stencil_view);
	}

	void D3D12Core::SetViewport(uint2 _resolution)
	{
		static uint2 resolution = 0;
		if (resolution == _resolution) return;

		CD3DX12_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(resolution.x), static_cast<float>(resolution.y));
		CD3DX12_RECT scissor(0, 0, static_cast<LONG>(resolution.x), static_cast<LONG>(resolution.y));
		command_device_.list_->RSSetViewports(1, &viewport);
		command_device_.list_->RSSetScissorRects(1, &scissor);
	}

	void D3D12Core::Present(uint _sync)
	{
		command_device_.list_->Close();

		ID3D12CommandList* lists[] = { command_device_.list_.Get() };
		command_device_.queue_->ExecuteCommandLists(_countof(lists), lists);

		swap_chain_->Present(_sync, 0);
	}

	void D3D12Core::WaitForCommandQueue()
	{
		command_device_.queue_->Signal(sync_.fence_.Get(), ++sync_.value_);
		if (sync_.fence_->GetCompletedValue() != sync_.value_)
		{
			sync_.fence_->SetEventOnCompletion(sync_.value_, sync_.event_);
			WaitForSingleObject(sync_.event_, INFINITE);
		}
	}

	ComPtr<ID3D12GraphicsCommandList> D3D12Core::CreateGraphicsCommandList(const D3D12_COMMAND_LIST_TYPE _type, ID3D12CommandAllocator* _allocator)
	{
		ComPtr<ID3D12GraphicsCommandList> list;
		ASSERT_MESSAGE(device_->CreateCommandList(0, _type, _allocator, nullptr, IID_PPV_ARGS(list.ReleaseAndGetAddressOf())), "GraphicsCore::CreateGraphicsCommandList failed.");
		return list;
	}

	ComPtr<ID3D12DescriptorHeap> D3D12Core::CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE _type, uint _num, bool _access_shader)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = _type;
		desc.NumDescriptors = _num;
		desc.Flags = _access_shader ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 0;

		ComPtr<ID3D12DescriptorHeap> descriptor_heap;
		ASSERT_MESSAGE(device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(descriptor_heap.ReleaseAndGetAddressOf())), "GraphicsCore::CreateDescriptorHeap failed.");
		return descriptor_heap;
	}

	ComPtr<ID3D12Resource> D3D12Core::CreateResource(const D3D12_HEAP_TYPE _type, uint _size, const void* _data)
	{
		ComPtr<ID3D12Resource> resource;
		ASSERT_MESSAGE(device_->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(_type), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(_size), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(resource.ReleaseAndGetAddressOf())), "GraphicsCore::CreateResource failed.");

		if(!_data) return resource;

		void* mapped = nullptr;
		CD3DX12_RANGE range(0, 0);
		ASSERT_MESSAGE(resource->Map(0, &range, &mapped), "ID3D12Resource::Map() failed.");
		memcpy(mapped, _data, _size);
		resource->Unmap(0, nullptr);

		return resource;
	}

	//VertexBuffer GraphicsCore::CreateVertexBuffer(const void* _data, uint _size, uint _stride)
	//{
	//	VertexBuffer v;
	//	v.resource_ = CreateResource(D3D12_HEAP_TYPE_UPLOAD, _size, _data);
	//	v.view_.BufferLocation = v.resource_->GetGPUVirtualAddress();
	//	v.view_.SizeInBytes = _size;
	//	v.view_.StrideInBytes = _stride;
	//	return v;
	//}

	//IndexBuffer GraphicsCore::CreateIndexBuffer(const void* _data, uint _size)
	//{
	//	IndexBuffer i;
	//	i.resource_ = CreateResource(D3D12_HEAP_TYPE_UPLOAD, _size, _data);
	//	i.view_.BufferLocation = i.resource_->GetGPUVirtualAddress();
	//	i.view_.SizeInBytes = _size;
	//	i.view_.Format = DXGI_FORMAT_R32_UINT;
	//	return i;
	//}

	//PipelineStateObject GraphicsCore::CreatePipelineStateObject()
	//{
	//	return PipelineStateObject{};
	//}

	//------------------------------------------------------------------------------------------------------------------------

	void D3D12Core::CreateDebugLayer()
	{
		ComPtr<ID3D12Debug> debug_layer;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_layer))))
		{
			debug_layer->EnableDebugLayer();
		}
	}

	void D3D12Core::CreateInfoQueue()
	{
		ComPtr<ID3D12InfoQueue> info_queue;
		if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&info_queue))))
		{
			D3D12_MESSAGE_SEVERITY sevirity[] =
			{
				D3D12_MESSAGE_SEVERITY_INFO
			};

			D3D12_MESSAGE_ID denyID[] =
			{
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
				D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
			};

			D3D12_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumSeverities = _countof(sevirity);
			filter.DenyList.pSeverityList = sevirity;
			filter.DenyList.NumIDs = _countof(denyID);
			filter.DenyList.pIDList = denyID;

			info_queue->PushStorageFilter(&filter);
		}
	}

	void D3D12Core::CreateDevice(IDXGIAdapter1* _adapter, const D3D_FEATURE_LEVEL _feature_level)
	{
		ASSERT_MESSAGE(D3D12CreateDevice(_adapter, _feature_level, IID_PPV_ARGS(&device_)), "GraphicsCore::CreateDevice failed.");
	}

	void D3D12Core::CreateSwapChain(IDXGIFactory4* _factory, HWND _hwnd)
	{
		DXGI_SWAP_CHAIN_DESC1 desc = {};
		desc.Width = constants::kResolution.x;
		desc.Height = constants::kResolution.y;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Stereo = false;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
		desc.BufferCount = snd::constants::kBackBufferCount;
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ComPtr<IDXGISwapChain1> swap_chain1;
		ASSERT_MESSAGE(_factory->CreateSwapChainForHwnd(command_device_.queue_.Get(), _hwnd, &desc, nullptr, nullptr, swap_chain1.ReleaseAndGetAddressOf()), "CreateSwapChainForHwnd failed.");

		// Alt + Enter 禁止
		_factory->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER);

		ASSERT_MESSAGE(swap_chain1.As(&swap_chain_), "IDXGISwapChain1::As failed.");
	}

	void D3D12Core::CreateRenderTarget()
	{
		render_target_.descriptor_heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2);
		render_target_.increment_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(render_target_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());
		D3D12_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		for (uint i = 0; i < constants::kBackBufferCount; ++i)
		{
			ASSERT_MESSAGE(swap_chain_->GetBuffer(i, IID_PPV_ARGS(&render_target_.buffers_[i])), "IDXGISwapChain4::GetBuffer failed.");
			device_->CreateRenderTargetView(render_target_.buffers_[i].Get(), &desc, cpu_handle);
			cpu_handle.Offset(1, render_target_.increment_size_);
		}
	}

	void D3D12Core::CreateDepthStencil()
	{
		depth_stencil_.descriptor_heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);

		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(depth_stencil_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());

		D3D12_RESOURCE_DESC resource_desc = {};
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resource_desc.Format = DXGI_FORMAT_D32_FLOAT;
		resource_desc.Width = constants::kResolution.x;
		resource_desc.Height = constants::kResolution.y;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.SampleDesc = { 1,0 };
		resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_HEAP_PROPERTIES heap_properties = {};
		heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_CLEAR_VALUE depth_clear_value = {};
		depth_clear_value.Format = resource_desc.Format;
		depth_clear_value.DepthStencil.Depth = 1.0f;
		depth_clear_value.DepthStencil.Stencil = 0;

		ASSERT_MESSAGE(device_->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depth_clear_value, IID_PPV_ARGS(&depth_stencil_.buffer_)), "GraphicsCore::CreateDepthStencil failed.");

		D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = resource_desc.Format;
		desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = D3D12_DSV_FLAG_NONE;
		desc.Texture2D.MipSlice = 0;

		device_->CreateDepthStencilView(depth_stencil_.buffer_.Get(), &desc, cpu_handle);
	}

	void D3D12Core::CreateFence()
	{
		ASSERT_MESSAGE(device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&sync_.fence_)), "GraphicsCore::CreateFence failed.");
		sync_.event_ = CreateEvent(nullptr, false, false, nullptr);
		sync_.value_ = 0;
	}

	ComPtr<IDXGIFactory4> D3D12Core::CreateFactory(uint _dxgi_flags)
	{
		ComPtr<IDXGIFactory4> factory;
		ASSERT_MESSAGE(CreateDXGIFactory2(_dxgi_flags, IID_PPV_ARGS(factory.ReleaseAndGetAddressOf())), "GraphicsCore::CreateFactory failed.");
		return factory;
	}

	ComPtr<IDXGIAdapter1> D3D12Core::CreateAdapter(IDXGIFactory4* _factory, const D3D_FEATURE_LEVEL _feature_level)
	{
		ComPtr<IDXGIAdapter1> adapter;
		for (UINT i = 0; _factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 adapter_desc;
			adapter->GetDesc1(&adapter_desc);

			// ハードウェアアダプターでサポートされているものを探す
			if (adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), _feature_level, __uuidof(ID3D12Device), nullptr)))
				break;
		}
		return adapter;
	}

	ComPtr<ID3D12CommandQueue> D3D12Core::CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE _type)
	{
		ComPtr<ID3D12CommandQueue> queue;
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = _type;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		ASSERT_MESSAGE(device_->CreateCommandQueue(&desc, IID_PPV_ARGS(queue.ReleaseAndGetAddressOf())), "GraphicsCore::CreateCommandQueue failed.");
		return queue;
	}

	ComPtr<ID3D12CommandAllocator> D3D12Core::CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE _type)
	{
		ComPtr<ID3D12CommandAllocator> allocator;
		ASSERT_MESSAGE(device_->CreateCommandAllocator(_type, IID_PPV_ARGS(allocator.ReleaseAndGetAddressOf())), "GraphicsCore::CreateCommandAllocator failed.");
		return allocator;
	}
}