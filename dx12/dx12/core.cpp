#include "core.h"
#include "utility.h"

using namespace Microsoft::WRL;

namespace snd::detail
{
	void core::initialize(HWND _hwnd, UINT _width, UINT _height)
	{
		// デバッグレイヤー有効化
		UINT dxgi_flags = 0u;
#if _DEBUG
		ComPtr<ID3D12Debug> debug_layer;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_layer))))
		{
			debug_layer->EnableDebugLayer();
			dxgi_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif
		// フィーチャーレベルの設定
		constexpr D3D_FEATURE_LEVEL kFeatureLevel = D3D_FEATURE_LEVEL_11_0;

		// ファクトリーの作成
		ComPtr<IDXGIFactory6> dxgi_factory;
		ASSERT_SUCCEEDED(CreateDXGIFactory2(dxgi_flags, IID_PPV_ARGS(&dxgi_factory)));
		utility::print("IDXGIFactory6 create successed.");

		// アダプターの検索
		ComPtr<IDXGIAdapter1> adapter;
		for (UINT i = 0; dxgi_factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
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
		utility::print("ID3D12Device create successed.");

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
		// コマンドの作成
		commands_.create(device_.Get());

		// スワップチェインの作成
		DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
		{
			swap_chain_desc.Width = _width;
			swap_chain_desc.Height = _height;
			swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swap_chain_desc.Stereo = false;
			swap_chain_desc.SampleDesc.Count = 1;
			swap_chain_desc.SampleDesc.Quality = 0;
			swap_chain_desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
			swap_chain_desc.BufferCount = render_target::kBufferCount;
			swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		}
		ComPtr<IDXGISwapChain1> swap_chain;
		ASSERT_SUCCEEDED(dxgi_factory->CreateSwapChainForHwnd(commands_.GetQueue(), _hwnd, &swap_chain_desc, nullptr, nullptr, swap_chain.GetAddressOf()));
		utility::print("IDXGISwapChain1 create successed.");

		// Alt + Enter 禁止
		dxgi_factory->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER);

		ASSERT_SUCCEEDED(swap_chain.As(&swap_chain_));
		utility::print("IDXGISwapChain4 create successed.");

		// レンダーターゲットの作成
		D3D12_RENDER_TARGET_VIEW_DESC render_target_view_desc = {};
		{
			render_target_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			render_target_view_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		}
		render_target_.descriptor_heap_.create(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2);
		for (int i = 0; i < render_target::kBufferCount; ++i)
		{
			render_target_.buffers_[i].create(swap_chain_.Get(), i);
			device_->CreateRenderTargetView(render_target_.buffers_[i].get(), &render_target_view_desc, render_target_.descriptor_heap_.get_cpu_handle());
			render_target_.descriptor_heap_.cpu_offset(1);
		}

		// デプスステンシルビューの作成
		CD3DX12_RESOURCE_DESC depth_buffer_desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, _width, _height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		D3D12_CLEAR_VALUE depth_clear_value = {};
		{
			depth_clear_value.Format = depth_buffer_desc.Format;
			depth_clear_value.DepthStencil.Depth = 1.0f;
			depth_clear_value.DepthStencil.Stencil = 0;
		}
		depth_stencil_.descriptor_heap_.create(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);
		depth_stencil_.buffer_.create(device_.Get(), D3D12_HEAP_TYPE_DEFAULT, D3D12_HEAP_FLAG_NONE, &depth_buffer_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depth_clear_value);
		D3D12_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc = {};
		{
			depth_stencil_view_desc.Format = DXGI_FORMAT_D32_FLOAT;
			depth_stencil_view_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depth_stencil_view_desc.Flags = D3D12_DSV_FLAG_NONE;
			depth_stencil_view_desc.Texture2D.MipSlice = 0;
		}
		device_->CreateDepthStencilView(depth_stencil_.buffer_.get(), &depth_stencil_view_desc, depth_stencil_.descriptor_heap_.get_cpu_handle());

		// フェンスの作成
		ASSERT_SUCCEEDED(device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&sync_.fence_)));
		sync_.event_ = CreateEvent(nullptr, false, false, nullptr);
		sync_.value_ = 0;
	}

	void core::uninitialize()
	{

	}
}