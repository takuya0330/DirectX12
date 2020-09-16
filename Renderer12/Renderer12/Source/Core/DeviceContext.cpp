#include "DeviceContext.h"
#include "Device.h"
#include "../Administrator.h"
#include "../Windows/Window.h"
#include "../Misc/Utility.h"

#include "../../ThirdParty/d3dx12/d3dx12.h"
#include "../../ThirdParty/imgui/imgui.h"
#include "../../ThirdParty/imgui/imgui_impl_win32.h"
#include "../../ThirdParty/imgui/imgui_impl_dx12.h"

namespace gl
{
	DeviceContext::~DeviceContext()
	{
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void DeviceContext::Initialize()
	{
		HRESULT hr = S_OK;
		HWND lHwnd = Administrator::Get<Window>()->GetHwnd();
		uint2 lWindowSize = Administrator::Get<Window>()->GetResolution();
		ID3D12Device5* lDevice = Administrator::Get<Device>()->GetDevice5();
		IDXGIFactory6* lFactory = Administrator::Get<Device>()->GetFactory6();

		constexpr D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{
			.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
			.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
			.NodeMask = 0
		};
		hr = lDevice->CreateCommandQueue(&lCommandQueueDesc, IID_PPV_ARGS(mCommandQueue.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));

		hr = lDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCommandAllocator.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));

		hr = lDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(mCommandList.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
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
		hr = lFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), lHwnd, &lSwapChainDesc, nullptr, nullptr, lSwapChain1.GetAddressOf());
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
		lSwapChain1.As(&mSwapChain4);

		constexpr D3D12_DESCRIPTOR_HEAP_DESC lBackBufferHeapDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			.NumDescriptors = kBackBufferNum,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			.NodeMask = 0
		};
		hr = lDevice->CreateDescriptorHeap(&lBackBufferHeapDesc, IID_PPV_ARGS(mBackBufferHeap.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
		mBackBufferHeap->SetName(L"BackBuffer");
		mBackBufferHeapSize = lDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE lBackBufferHandle(mBackBufferHeap->GetCPUDescriptorHandleForHeapStart());
		constexpr D3D12_RENDER_TARGET_VIEW_DESC lRTVDesc{
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
		};
		for (uint i = 0; i < kBackBufferNum; ++i)
		{
			hr = mSwapChain4->GetBuffer(i, IID_PPV_ARGS(mBackBuffers[i].GetAddressOf()));
			_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
			mBackBuffers[i]->SetName(L"BackBuffer");
			lDevice->CreateRenderTargetView(mBackBuffers[i].Get(), &lRTVDesc, lBackBufferHandle);
			lBackBufferHandle.Offset(mBackBufferHeapSize);
		}

		constexpr D3D12_DESCRIPTOR_HEAP_DESC lDepthStencilHeapDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
			.NumDescriptors = 1,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			.NodeMask = 0
		};
		hr = lDevice->CreateDescriptorHeap(&lDepthStencilHeapDesc, IID_PPV_ARGS(mDepthStencilHeap.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
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
		hr = lDevice->CreateCommittedResource(&lHeapProperties, D3D12_HEAP_FLAG_NONE, &lResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &lClearValue, IID_PPV_ARGS(mDepthBuffer.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
		constexpr D3D12_DEPTH_STENCIL_VIEW_DESC lDSVDesc{
			.Format = DXGI_FORMAT_D32_FLOAT,
			.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
			.Flags = D3D12_DSV_FLAG_NONE,
			.Texture2D = {.MipSlice = 0 }
		};
		lDevice->CreateDepthStencilView(mDepthBuffer.Get(), &lDSVDesc, lDepthStencilHandle);

		hr = lDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
		mFenceEvent = CreateEvent(nullptr, false, false, nullptr);
		mFenceValue = 0;

		constexpr D3D12_DESCRIPTOR_HEAP_DESC lImGuiHeapDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			.NumDescriptors = 1,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
			.NodeMask = 0
		};
		hr = lDevice->CreateDescriptorHeap(&lImGuiHeapDesc, IID_PPV_ARGS(mImGuiHeap.GetAddressOf()));
		_ASSERTION(SUCCEEDED(hr), hResultTrace(hr));
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
		ImGui_ImplWin32_Init(lHwnd);
		ImGui_ImplDX12_Init(lDevice, DeviceContext::kBackBufferNum, DXGI_FORMAT_R8G8B8A8_UNORM, mImGuiHeap.Get(), mImGuiHeap->GetCPUDescriptorHandleForHeapStart(), mImGuiHeap->GetGPUDescriptorHandleForHeapStart());
	}

	void DeviceContext::NewFrame()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
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

	void DeviceContext::SetViewport(const uint2& _WindowSize)
	{
		mViewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(_WindowSize.x), static_cast<float>(_WindowSize.y));
		mScissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(_WindowSize.x), static_cast<LONG>(_WindowSize.y));
		mCommandList->RSSetViewports(1, &mViewport);
		mCommandList->RSSetScissorRects(1, &mScissorRect);
	}

	void DeviceContext::GUIRender()
	{
		mCommandList->SetDescriptorHeaps(1, mImGuiHeap.GetAddressOf());
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), mCommandList.Get());
	}

	void DeviceContext::Execute()
	{
		mCommandList->Close();
		mCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)mCommandList.GetAddressOf());
	}

	void DeviceContext::GUIConfig()
	{
		// Update and Render additional Platform Windows
		if (ImGuiIO& lInfo = ImGui::GetIO(); lInfo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(NULL, (void*)mCommandList.Get());
		}
	}

	void DeviceContext::Present(uint _Vsync)
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
}