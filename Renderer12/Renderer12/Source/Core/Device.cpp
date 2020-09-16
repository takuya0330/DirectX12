#include "Device.h"
#include "../Misc/Utility.h"

namespace gl
{
	void Device::Initialize()
	{
		HRESULT hr = S_OK;
#if _DEBUG
		constexpr uint lDXGIFlags = DXGI_CREATE_FACTORY_DEBUG;
		ComPtr<ID3D12Debug> lDebug;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(lDebug.GetAddressOf()))))
			lDebug->EnableDebugLayer();
#else
		constexpr uint lDXGIFlags = 0;
#endif
		ComPtr<IDXGIFactory2> lFactory2;
		hr = CreateDXGIFactory2(lDXGIFlags, IID_PPV_ARGS(lFactory2.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
		lFactory2.As(&mFactory6);

		ComPtr<IDXGIAdapter4> lAdapter;
		DXGI_ADAPTER_DESC3 lAdapterDesc{};
		for (uint i = 0;; ++i)
		{
			if (mFactory6->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(lAdapter.GetAddressOf())) == DXGI_ERROR_NOT_FOUND)
			{
				mUseWrapAdapter = true;
				break;
			}

			lAdapter->GetDesc3(&lAdapterDesc);

			if (!(lAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE))
				break;
		}
		if (mUseWrapAdapter)
		{
			hr = mFactory6->EnumWarpAdapter(IID_PPV_ARGS(lAdapter.GetAddressOf()));
			_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
		}

		ComPtr<ID3D12Device> lDevice;
		hr = D3D12CreateDevice(lAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(lDevice.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
		lDevice.As(&mDevice5);
#if _DEBUG
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
#endif
		constexpr D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{
			.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
			.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
			.NodeMask = 0
		};
		hr = mDevice5->CreateCommandQueue(&lCommandQueueDesc, IID_PPV_ARGS(mCommandQueue.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));

		hr = mDevice5->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCommandAllocator.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
		
		hr = mDevice5->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(mCommandList.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
		mCommandList->Close();

		hr = mDevice5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
		mFenceEvent = CreateEvent(nullptr, false, false, nullptr);
		mFenceValue = 0;
	}

	void Device::BarrierTransition(ID3D12Resource* _Resource, D3D12_RESOURCE_STATES _Before, D3D12_RESOURCE_STATES _Affter)
	{
		D3D12_RESOURCE_BARRIER lBarrier{
			.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
			.Transition = {
				.pResource = _Resource,
				.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
				.StateBefore = _Before,
				.StateAfter = _Affter
			}
		};
		mCommandList->ResourceBarrier(1, &lBarrier);
	}

	void Device::ExecuteCommandList()
	{
		mCommandList->Close();
		mCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)mCommandList.GetAddressOf());
	}

	void Device::WaitFence()
	{
		mCommandQueue->Signal(mFence.Get(), ++mFenceValue);
		if (mFence->GetCompletedValue() != mFenceValue)
		{
			mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);
			WaitForSingleObject(mFenceEvent, INFINITE);
		}
	}

	void Device::ReportLiveObjects()
	{
		ComPtr<ID3D12DebugDevice> lDebug;
		mDevice5->QueryInterface(lDebug.GetAddressOf());
		lDebug->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
	}
}