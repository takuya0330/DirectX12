#include "device_context.h"

#include "../../system.h"
#include "../windows/window.h"
#include "../device/device.h"
#include "../utility/utility.h"

#include "../../third_party/d3dx12/d3dx12.h"
#include "../../third_party/imgui/imgui.h"
#include "../../third_party/imgui/imgui_impl_win32.h"
#include "../../third_party/imgui/imgui_impl_dx12.h"

namespace detail
{
	DeviceContext::~DeviceContext()
	{
#if _DEBUG
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
#endif
	}

	void DeviceContext::Initialize()
	{
		HRESULT hr = S_OK;
		detail::Window* lWindow = System::Get<detail::Window>();
		uint2 lWindowSize = lWindow->GetSize();
		detail::Device* lDevice = System::Get<detail::Device>();
		ID3D12Device5* lD3D12Device = lDevice->GetDevice5();

		constexpr D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{
			.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
			.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
			.NodeMask = 0
		};
		hr = lD3D12Device->CreateCommandQueue(&lCommandQueueDesc, IID_PPV_ARGS(mCommandQueue.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateCommandQueue is failed.");

		hr = lD3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCommandAllocator.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateCommandAllocator is failed.");

		hr = lD3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(mCommandList.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateCommandList is failed.");
		mCommandList->Close();

		DXGI_SWAP_CHAIN_DESC1 lSwapChainDesc{
			.Width = lWindowSize.x,
			.Height = lWindowSize.y,
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.Stereo = false,
			.SampleDesc = {.Count = 1, .Quality = 0 },
			.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
			.BufferCount = kBackBufferNum,
			.Scaling = DXGI_SCALING_STRETCH,
			.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
			.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
			.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		};
		ComPtr<IDXGISwapChain1> lSwapChain1;
		hr = lDevice->GetFactory6()->CreateSwapChainForHwnd(mCommandQueue.Get(), lWindow->GetHwnd(), &lSwapChainDesc, nullptr, nullptr, lSwapChain1.GetAddressOf());
		_ERROR_TRACE(SUCCEEDED(hr), "IDXGIFactory6::CreateSwapChainForHwnd is failed.");
		lSwapChain1.As(&mSwapChain4);

		constexpr D3D12_DESCRIPTOR_HEAP_DESC lBackBufferHeapDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			.NumDescriptors = kBackBufferNum,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			.NodeMask = 0
		};
		hr = lD3D12Device->CreateDescriptorHeap(&lBackBufferHeapDesc, IID_PPV_ARGS(mBackBufferHeap.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateDescriptorHeap is failed.");
		mBackBufferHeap->SetName(L"BackBuffer");
		mBackBufferHeapSize = lD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE lBackBufferHandle(mBackBufferHeap->GetCPUDescriptorHandleForHeapStart());
		constexpr D3D12_RENDER_TARGET_VIEW_DESC lRTVDesc{
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
		};
		for (UINT i = 0; i < kBackBufferNum; ++i)
		{
			hr = mSwapChain4->GetBuffer(i, IID_PPV_ARGS(mBackBuffers[i].GetAddressOf()));
			_ERROR_TRACE(SUCCEEDED(hr), "IDXGISwapChain4::GetBuffer is failed.");
			mBackBuffers[i]->SetName(L"BackBuffer");
			lD3D12Device->CreateRenderTargetView(mBackBuffers[i].Get(), &lRTVDesc, lBackBufferHandle);
			lBackBufferHandle.Offset(mBackBufferHeapSize);
		}

		constexpr D3D12_DESCRIPTOR_HEAP_DESC lDepthStencilHeapDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
			.NumDescriptors = 1,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			.NodeMask = 0
		};
		hr = lD3D12Device->CreateDescriptorHeap(&lDepthStencilHeapDesc, IID_PPV_ARGS(mDepthStencilHeap.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateDescriptorHeap is failed.");
		CD3DX12_CPU_DESCRIPTOR_HANDLE lDepthStencilHandle(mDepthStencilHeap->GetCPUDescriptorHandleForHeapStart());
		D3D12_RESOURCE_DESC lResourceDesc{
			.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			.Width = lWindowSize.x,
			.Height = lWindowSize.y,
			.DepthOrArraySize = 1,
			.Format = DXGI_FORMAT_D32_FLOAT,
			.SampleDesc = {.Count = 1, .Quality = 0 },
			.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		};
		constexpr D3D12_HEAP_PROPERTIES lHeapProperties{
			.Type = D3D12_HEAP_TYPE_DEFAULT,
			.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN
		};
		constexpr D3D12_CLEAR_VALUE lClearValue{
			.Format = DXGI_FORMAT_D32_FLOAT,
			.DepthStencil = {.Depth = 1.0f, .Stencil = 0}
		};
		hr = lD3D12Device->CreateCommittedResource(&lHeapProperties, D3D12_HEAP_FLAG_NONE, &lResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &lClearValue, IID_PPV_ARGS(mDepthBuffer.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateCommittedResource is failed.");
		constexpr D3D12_DEPTH_STENCIL_VIEW_DESC lDSVDesc{
			.Format = DXGI_FORMAT_D32_FLOAT,
			.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
			.Flags = D3D12_DSV_FLAG_NONE,
			.Texture2D = {.MipSlice = 0 }
		};
		lD3D12Device->CreateDepthStencilView(mDepthBuffer.Get(), &lDSVDesc, lDepthStencilHandle);

		hr = lD3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateFence is failed.");
		mFenceEvent = CreateEvent(nullptr, false, false, nullptr);
		mFenceValue = 0;

#if _DEBUG
		constexpr D3D12_DESCRIPTOR_HEAP_DESC lImGuiHeapDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			.NumDescriptors = 1,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
			.NodeMask = 0
		};
		hr = lD3D12Device->CreateDescriptorHeap(&lImGuiHeapDesc, IID_PPV_ARGS(mImGuiHeap.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateDescriptorHeap is failed.");
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& lInfo = ImGui::GetIO(); (void)lInfo;
		lInfo.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
		lInfo.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
		lInfo.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 18.0f, NULL, lInfo.Fonts->GetGlyphRangesJapanese());
		ImGui::StyleColorsDark();
		ImGuiStyle& lStyle = ImGui::GetStyle();
		if (lInfo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			lStyle.WindowRounding = 0.0f;
			lStyle.FrameRounding = 1;
			lStyle.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		ImGui_ImplWin32_Init(lWindow->GetHwnd());
		ImGui_ImplDX12_Init(lD3D12Device, DeviceContext::kBackBufferNum, DXGI_FORMAT_R8G8B8A8_UNORM, mImGuiHeap.Get(), mImGuiHeap->GetCPUDescriptorHandleForHeapStart(), mImGuiHeap->GetGPUDescriptorHandleForHeapStart());
#endif
	}

	void DeviceContext::ImGuiUpdate()
	{
#if _DEBUG
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
#endif
	}

	void DeviceContext::Reset()
	{
		mBackBufferIndex = mSwapChain4->GetCurrentBackBufferIndex();
		mCommandAllocator->Reset();
		mCommandList->Reset(mCommandAllocator.Get(), nullptr);
	}

	void DeviceContext::BarrierTransition(D3D12_RESOURCE_STATES _Before, D3D12_RESOURCE_STATES _Affter)
	{
		D3D12_RESOURCE_BARRIER lBarrier{
			.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			.Transition = {
				.pResource = mBackBuffers[mBackBufferIndex].Get(),
				.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
				.StateBefore = _Before,
				.StateAfter = _Affter
			}
		};
		mCommandList->ResourceBarrier(1, &lBarrier);
	}

	void DeviceContext::ClearScreen(const float* _ClearColor)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE lRTV(mBackBufferHeap->GetCPUDescriptorHandleForHeapStart(), mBackBufferIndex, mBackBufferHeapSize);
		CD3DX12_CPU_DESCRIPTOR_HANDLE lDSV(mDepthStencilHeap->GetCPUDescriptorHandleForHeapStart());
		mCommandList->OMSetRenderTargets(1, &lRTV, true, &lDSV);
		mCommandList->ClearRenderTargetView(lRTV, _ClearColor, 0, nullptr);
		mCommandList->ClearDepthStencilView(lDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}

	void DeviceContext::SetViewport(UINT _Width, UINT _Height)
	{
		mViewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(_Width), static_cast<float>(_Height));
		mScissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(_Height), static_cast<LONG>(_Height));
		mCommandList->RSSetViewports(1, &mViewport);
		mCommandList->RSSetScissorRects(1, &mScissorRect);
	}

	void DeviceContext::ImGuiRender()
	{
#if _DEBUG
		mCommandList->SetDescriptorHeaps(1, mImGuiHeap.GetAddressOf());
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), mCommandList.Get());
#endif
	}

	void DeviceContext::Execute()
	{
		mCommandList->Close();
		mCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)mCommandList.GetAddressOf());
	}

	void DeviceContext::ImGuiConfig()
	{
#if _DEBUG
		// Update and Render additional Platform Windows
		if (ImGuiIO& lInfo = ImGui::GetIO(); lInfo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(NULL, (void*)mCommandList.Get());
		}
#endif
	}

	void DeviceContext::Present(UINT _Vsync)
	{
		mSwapChain4->Present(_Vsync, 0);
	}

	void DeviceContext::WaitFence()
	{
		mCommandQueue->Signal(mFence.Get(), ++mFenceValue);
		if (mFence->GetCompletedValue() != mFenceValue)
		{
			mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);
			WaitForSingleObject(mFenceEvent, INFINITE);
		}
	}

	void DeviceContext::ImGuiFunction(const char* _Title, std::function<void()> _Function)
	{
#if _DEBUG
		if (ImGui::Begin(_Title))
		{
			_Function();
		}
		ImGui::End();
#endif
	}
}