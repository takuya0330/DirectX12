#include "Device.h"
#include "Utility.h"

using namespace Microsoft::WRL;
namespace gl
{
	void Device::Initialize()
	{
#if _DEBUG
		constexpr uint lDxgiFlags = DXGI_CREATE_FACTORY_DEBUG;
		CreateDebugLayer();
#else
		constexpr uint lDxgiFlags = 0;
#endif
		CreateFactory(lDxgiFlags);
		
		ComPtr<IDXGIAdapter4> lAdapter = SearchAdapter(mFactory6.Get());
		
		CreateDevice(lAdapter.Get());
#if _DEBUG
		CreateInfoQueue();
#endif
		CreateCommandQueue();

		CreateCommandAllocator();

		CreateCommandList();
	}

	void Device::ReportLiveObjects()
	{
		ComPtr<ID3D12DebugDevice> lDebug;
		mDevice5->QueryInterface(lDebug.GetAddressOf());
		lDebug->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
	}

	void Device::CreateDebugLayer()
	{
		ComPtr<ID3D12Debug> lDebug;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(lDebug.GetAddressOf()))))
		{
			lDebug->EnableDebugLayer();
		}
	}

	void Device::CreateInfoQueue()
	{
		ComPtr<ID3D12InfoQueue> lInfoQueue;
		if (SUCCEEDED(mDevice5->QueryInterface(IID_PPV_ARGS(lInfoQueue.GetAddressOf()))))
		{
			D3D12_MESSAGE_SEVERITY lSevirity[] = {
				D3D12_MESSAGE_SEVERITY_INFO
			};
			D3D12_MESSAGE_ID lDenyID[] = {
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
				D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
			};
			D3D12_INFO_QUEUE_FILTER lFilter{
				.DenyList = {
					.NumSeverities = _countof(lSevirity),
					.pSeverityList = lSevirity,
					.NumIDs = _countof(lDenyID),
					.pIDList = lDenyID
				}
			};
			lInfoQueue->PushStorageFilter(&lFilter);
		}
	}

	void Device::CreateFactory(uint _DxgiFlags)
	{
		ComPtr<IDXGIFactory2> lFactory2;
		_ASSERT_EXPR_A(SUCCEEDED(CreateDXGIFactory2(_DxgiFlags, IID_PPV_ARGS(&lFactory2))), "CreateDXGIFactory2 is failed.");
		lFactory2.As(&mFactory6);
	}

	void Device::CreateDevice(IDXGIAdapter4* _Adapter)
	{
		ComPtr<ID3D12Device> lDevice;
		_ASSERT_EXPR_A(SUCCEEDED(D3D12CreateDevice(_Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(lDevice.GetAddressOf()))), "D3D12CreateDevice is failed.");
		lDevice.As(&mDevice5);
	}

	void Device::CreateCommandQueue()
	{
		constexpr D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{
			.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
			.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
			.NodeMask = 0
		};
		_ASSERT_EXPR_A(SUCCEEDED(mDevice5->CreateCommandQueue(&lCommandQueueDesc, IID_PPV_ARGS(mCommandQueue.GetAddressOf()))), "CreateCommandQueue is failed.");
	}

	void Device::CreateCommandAllocator()
	{
		_ASSERT_EXPR_A(SUCCEEDED(mDevice5->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCommandAllocator.GetAddressOf()))), "CreateCommandAllocator is failed.");
	}

	void Device::CreateCommandList()
	{
		_ASSERT_EXPR_A(SUCCEEDED(mDevice5->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(mCommandList.GetAddressOf()))), "CreateCommandList is failed.");
		mCommandList->Close();
	}

	ComPtr<IDXGIAdapter4> Device::SearchAdapter(IDXGIFactory6* _factory)
	{
		ComPtr<IDXGIAdapter4> lAdapter;
		DXGI_ADAPTER_DESC3 lAdapterDesc = {};
		for (uint i = 0;; ++i)
		{
			if (_factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(lAdapter.GetAddressOf())) == DXGI_ERROR_NOT_FOUND)
			{
				mUseWrapAdapter = true;
				break;
			}

			lAdapter->GetDesc3(&lAdapterDesc);

			if (!(lAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)) break;
		}

		if (mUseWrapAdapter) _ASSERT_EXPR_A(SUCCEEDED(_factory->EnumWarpAdapter(IID_PPV_ARGS(lAdapter.GetAddressOf()))), "EnumWarpAdapter is failed.");

		return lAdapter;
	}
}