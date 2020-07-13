#include "device.h"
#include "utility.h"

using namespace Microsoft::WRL;
namespace snd::detail
{
	void Device::Create()
	{
		// デバッグレイヤー有効化
#if _DEBUG
		constexpr UINT dxgi_flags = DXGI_CREATE_FACTORY_DEBUG;
		ComPtr<ID3D12Debug> debug_layer;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_layer))))
		{
			debug_layer->EnableDebugLayer();
		}
#else 
		constexpr UINT dxgi_flags = 0;
#endif

		// フィーチャーレベルの設定
		constexpr D3D_FEATURE_LEVEL kFeatureLevel = D3D_FEATURE_LEVEL_11_0;

		// ファクトリーの作成
		ComPtr<IDXGIFactory2> factory;
		ASSERT_SUCCEEDED(CreateDXGIFactory2(dxgi_flags, IID_PPV_ARGS(&factory)));
		ASSERT_SUCCEEDED(factory.As(&factory_));

		// アダプターの検索
		ComPtr<IDXGIAdapter1> adapter;
		for (UINT i = 0; factory_->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 adapter_desc;
			adapter->GetDesc1(&adapter_desc);

			// ハードウェアアダプターでサポートされているものを探す
			if (adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), kFeatureLevel, __uuidof(ID3D12Device), nullptr)))
				break;
		}

		// デバイスの作成
		ASSERT_SUCCEEDED(D3D12CreateDevice(adapter.Get(), kFeatureLevel, IID_PPV_ARGS(&device_)));

		// デバッグ設定
		{
#if _DEBUG
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
#endif
		}
	}
}