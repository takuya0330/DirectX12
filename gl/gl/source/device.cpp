#include "device.h"
#include "assertion.h"
#include "mathematics.h"

using namespace Microsoft::WRL;
namespace gl
{
	device::device()
	{
#if _DEBUG
		constexpr UINT dxgi_flags = DXGI_CREATE_FACTORY_DEBUG;
		create_debug_layer();
#else
		constexpr UINT dxgi_flags = 0;
#endif

		create_factory(dxgi_flags);

		ComPtr<IDXGIAdapter4> adapter = search_adapter(factory_.Get(), D3D_FEATURE_LEVEL_11_0);

		create_d3d12device(adapter.Get(), D3D_FEATURE_LEVEL_11_0);

#if _DEBUG
		create_info_queue();
#endif

		create_command_queue(D3D12_COMMAND_LIST_TYPE_DIRECT);

		create_command_allocator(D3D12_COMMAND_LIST_TYPE_DIRECT);

		create_command_list(D3D12_COMMAND_LIST_TYPE_DIRECT);
	}

	void device::create_debug_layer()
	{
		ComPtr<ID3D12Debug> debug_layer;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_layer))))
		{
			debug_layer->EnableDebugLayer();
		}
	}

	void device::create_info_queue()
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

	void device::create_factory(UINT _dxgi_flags)
	{
		ComPtr<IDXGIFactory2> factory;
		_ASSERT_EXPR_A(SUCCEEDED(CreateDXGIFactory2(_dxgi_flags, IID_PPV_ARGS(&factory))), "CreateDXGIFactory2 is failed.");
		factory.As(&factory_);
	}

	void device::create_d3d12device(IDXGIAdapter4* _adapter, const D3D_FEATURE_LEVEL _feature_level)
	{
		ComPtr<ID3D12Device> device;
		_ASSERT_EXPR_A(SUCCEEDED(D3D12CreateDevice(_adapter, _feature_level, IID_PPV_ARGS(&device))), "D3D12CreateDevice is failed.");
		device.As(&device_);
	}

	void device::create_command_queue(D3D12_COMMAND_LIST_TYPE _command_list_type)
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = _command_list_type;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		_ASSERT_EXPR_A(SUCCEEDED(device_->CreateCommandQueue(&desc, IID_PPV_ARGS(&command_queue_))), "CreateCommandQueue is failed.");
	}

	void device::create_command_allocator(D3D12_COMMAND_LIST_TYPE _command_list_type)
	{
		_ASSERT_EXPR_A(SUCCEEDED(device_->CreateCommandAllocator(_command_list_type, IID_PPV_ARGS(&command_allocator_))), "CreateCommandAllocator is failed.");
	}

	void device::create_command_list(D3D12_COMMAND_LIST_TYPE _command_list_type)
	{
		_ASSERT_EXPR_A(SUCCEEDED(device_->CreateCommandList(0, _command_list_type, command_allocator_.Get(), nullptr, IID_PPV_ARGS(&command_list_))), "CreateCommandList is failed.");
		command_list_->Close();
	}

	ComPtr<IDXGIAdapter4> device::search_adapter(IDXGIFactory6* _factory, const D3D_FEATURE_LEVEL _feature_level)
	{
		ComPtr<IDXGIAdapter4> adapter;
		DXGI_ADAPTER_DESC3 adapter_desc = {};
		for (uint i = 0;; ++i)
		{
			if (_factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) == DXGI_ERROR_NOT_FOUND)
			{
				use_wrap_adapter_ = true;
				break;
			}

			adapter->GetDesc3(&adapter_desc);

			if (!(adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)) break;

		}

		if (use_wrap_adapter_) _ASSERT_EXPR_A(SUCCEEDED(_factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter))), "EnumWarpAdapter is failed.");

		return adapter;
	}
}