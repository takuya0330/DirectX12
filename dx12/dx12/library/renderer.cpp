#include "renderer.h"
#include "utility.h"

#include "../external/d3dx12/d3dx12.h"

using namespace Microsoft::WRL;
namespace snd::detail
{
	void Renderer::Create(HWND _hwnd, ID3D12Device* _device, IDXGIFactory6* _factory)
	{
		// コマンドキューの作成
		D3D12_COMMAND_QUEUE_DESC queue_desc = {};
		{
			queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;				// コマンドリストと合わせる
			queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// プライオリティは特に指定なし
			queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;				// タイムアウト無し
			queue_desc.NodeMask = 0;												// アダプターが１つなら０でよし
		}
		ASSERT_SUCCEEDED(_device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_.queue_)));

		// コマンドアロケーターの作成
		ASSERT_SUCCEEDED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_.allocator_)));

		// コマンドリストの作成
		ASSERT_SUCCEEDED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_.allocator_.Get(), nullptr, IID_PPV_ARGS(&command_.list_)));
		command_.list_->Close();

		// スワップチェインの作成
		DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
		{
			swap_chain_desc.Width = constants::kResolution.x;
			swap_chain_desc.Height = constants::kResolution.y;
			swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swap_chain_desc.Stereo = false;
			swap_chain_desc.SampleDesc.Count = 1;
			swap_chain_desc.SampleDesc.Quality = 0;
			swap_chain_desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
			swap_chain_desc.BufferCount = snd::constants::kBackBufferCount;
			swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		}
		ComPtr<IDXGISwapChain1> swap_chain;
		ASSERT_SUCCEEDED(_factory->CreateSwapChainForHwnd(command_.queue_.Get(), _hwnd, &swap_chain_desc, nullptr, nullptr, swap_chain.GetAddressOf()));

		// Alt + Enter 禁止
		_factory->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER);

		ASSERT_SUCCEEDED(swap_chain.As(&swap_chain_));

		// レンダーターゲットの作成
		D3D12_DESCRIPTOR_HEAP_DESC rt_dh_desc = {};
		{
			rt_dh_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;				// 作成するビューの指定
			rt_dh_desc.NumDescriptors = constants::kBackBufferCount;	// 表裏の2つ
			rt_dh_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;			// 特になし
			rt_dh_desc.NodeMask = 0;												// 複数のGPUがある場合に識別するビットフラグ 
		}
		ASSERT_SUCCEEDED(_device->CreateDescriptorHeap(&rt_dh_desc, IID_PPV_ARGS(&render_target_.descriptor_heap_)));
		CD3DX12_CPU_DESCRIPTOR_HANDLE rt_cpu_handle(render_target_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());
		render_target_.increment_size_ = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
		{
			rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		}
		for (uint i = 0; i < constants::kBackBufferCount; ++i)
		{
			ASSERT_SUCCEEDED(swap_chain_->GetBuffer(i, IID_PPV_ARGS(&render_target_.buffers_[i])));
			_device->CreateRenderTargetView(render_target_.buffers_[i].Get(), &rtv_desc, rt_cpu_handle);
			rt_cpu_handle.Offset(1, render_target_.increment_size_);
		}

		// デプスステンシルビューの作成
		D3D12_DESCRIPTOR_HEAP_DESC ds_dh_desc = {};
		{
			ds_dh_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	
			ds_dh_desc.NumDescriptors = 1;							
			ds_dh_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ds_dh_desc.NodeMask = 0;									
		}
		ASSERT_SUCCEEDED(_device->CreateDescriptorHeap(&ds_dh_desc, IID_PPV_ARGS(&depth_stencil_.descriptor_heap_)));
		CD3DX12_CPU_DESCRIPTOR_HANDLE ds_cpu_handle(depth_stencil_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());

		CD3DX12_RESOURCE_DESC db_desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, constants::kResolution.x, constants::kResolution.y, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		D3D12_CLEAR_VALUE depth_clear_value = {};
		{
			depth_clear_value.Format = db_desc.Format;
			depth_clear_value.DepthStencil.Depth = 1.0f;
			depth_clear_value.DepthStencil.Stencil = 0;
		}
		ASSERT_SUCCEEDED(_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &db_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depth_clear_value, IID_PPV_ARGS(&depth_stencil_.buffer_)));

		D3D12_DEPTH_STENCIL_VIEW_DESC ds_desc = {};
		{
			ds_desc.Format = DXGI_FORMAT_D32_FLOAT;
			ds_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			ds_desc.Flags = D3D12_DSV_FLAG_NONE;
		};
		_device->CreateDepthStencilView(depth_stencil_.buffer_.Get(), &ds_desc, ds_cpu_handle);

		// フェンスの作成
		ASSERT_SUCCEEDED(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&sync_.fence_)));
		sync_.event_ = CreateEvent(nullptr, false, false, nullptr);
		sync_.value_ = 0;
	}

	void Renderer::Reset()
	{
		current_back_buffer_ = swap_chain_->GetCurrentBackBufferIndex();
		command_.allocator_->Reset();
		command_.list_->Reset(command_.allocator_.Get(), nullptr);
	}

	void Renderer::Barrier(D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _after)
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(render_target_.buffers_[current_back_buffer_].Get(), _before, _after);
		command_.list_->ResourceBarrier(1, &barrier);
	}

	void Renderer::Clear(const float* _clear_color)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE render_target_view(render_target_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), current_back_buffer_, render_target_.increment_size_);
		CD3DX12_CPU_DESCRIPTOR_HANDLE depth_stencil_view(depth_stencil_.descriptor_heap_->GetCPUDescriptorHandleForHeapStart());
		command_.list_->OMSetRenderTargets(1, &render_target_view, true, &depth_stencil_view);

		command_.list_->ClearRenderTargetView(render_target_view, _clear_color, 0, nullptr);
		command_.list_->ClearDepthStencilView(depth_stencil_view, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}

	void Renderer::SetViewport(uint2 _Resolution)
	{
		CD3DX12_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(_Resolution.x), static_cast<float>(_Resolution.y));
		CD3DX12_RECT scissor(0, 0, static_cast<LONG>(_Resolution.x), static_cast<LONG>(_Resolution.y));

		command_.list_->RSSetViewports(1, &viewport);
		command_.list_->RSSetScissorRects(1, &scissor);
	}

	void Renderer::Present(uint _sync)
	{
		command_.list_->Close();

		ID3D12CommandList* lists[] = { command_.list_.Get() };
		command_.queue_->ExecuteCommandLists(_countof(lists), lists);

		swap_chain_->Present(_sync, 0);
	}

	void Renderer::WaitGPU()
	{
		command_.queue_->Signal(sync_.fence_.Get(), ++sync_.value_);
		if (sync_.fence_->GetCompletedValue() != sync_.value_)
		{
			sync_.fence_->SetEventOnCompletion(sync_.value_, sync_.event_);

			// イベントが発生するまで待つ
			WaitForSingleObject(sync_.event_, INFINITE);
		}
	}
}