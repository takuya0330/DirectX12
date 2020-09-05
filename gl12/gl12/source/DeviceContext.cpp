#include "DeviceContext.h"
#include "Utility.h"
#include "Administrator.h"
#include "Window.h"
#include "Device.h"

#include "../external/d3dx12/d3dx12.h"

namespace gl
{
	void DeviceContext::Initialize()
	{
		HWND lHwnd = gl::Administrator::Get<gl::Window>()->GetHwnd();
		ID3D12Device5* lDevice = gl::Administrator::Get<gl::Device>()->GetDevice5();
		IDXGIFactory6* lFactory = gl::Administrator::Get<gl::Device>()->GetFactory6();

		CreateCommandQueue(lDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);

		CreateCommandAllocator(lDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);

		CreateCommandList(lDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);

		CreateSwapChain(lHwnd, lFactory);

		CreateRenderTarget(lDevice);

		CreateDepthStencil(lDevice);

		CreateFence(lDevice);
	}

	void DeviceContext::Reset()
	{
		mBackBufferIndex = mSwapChain4->GetCurrentBackBufferIndex();
		mCommandAllocator->Reset();
		mCommandList->Reset(mCommandAllocator.Get(), nullptr);
	}

	void DeviceContext::SetBarrier(D3D12_RESOURCE_STATES _Before, D3D12_RESOURCE_STATES _Affter)
	{
		D3D12_RESOURCE_BARRIER lBarrier{
			.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			.Transition = {
				.pResource = mRT.mBuffers[mBackBufferIndex].Get(),
				.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
				.StateBefore = _Before,
				.StateAfter = _Affter
			}
		};
		mCommandList->ResourceBarrier(1, &lBarrier);
	}

	void DeviceContext::ClearScreen(const float* _ClearColor)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE lRTV(mRT.mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), mBackBufferIndex, mRT.mDescriptorSize);
		CD3DX12_CPU_DESCRIPTOR_HANDLE lDSV(mDS.mDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
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

	void DeviceContext::Execute()
	{
		mCommandList->Close();
		mCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList*const*)mCommandList.GetAddressOf());
	}

	void DeviceContext::Present(uint _Vsync)
	{
		mSwapChain4->Present(_Vsync, 0);
	}

	void DeviceContext::WaitGpu()
	{
		mCommandQueue->Signal(mSync.mFence.Get(), ++mSync.mValue);
		if (mSync.mFence->GetCompletedValue() != mSync.mValue)
		{
			mSync.mFence->SetEventOnCompletion(mSync.mValue, mSync.mEvent);
			WaitForSingleObject(mSync.mEvent, INFINITE);
		}
	}

	void DeviceContext::CreateCommandQueue(ID3D12Device* _Device, D3D12_COMMAND_LIST_TYPE _Type)
	{
		D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{
			.Type = _Type,
			.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
			.NodeMask = 0
		};
		_ASSERT_EXPR_A(SUCCEEDED(_Device->CreateCommandQueue(&lCommandQueueDesc, IID_PPV_ARGS(mCommandQueue.GetAddressOf()))), "CreateCommandQueue is failed.");
	}

	void DeviceContext::CreateCommandAllocator(ID3D12Device* _Device, D3D12_COMMAND_LIST_TYPE _Type)
	{
		_ASSERT_EXPR_A(SUCCEEDED(_Device->CreateCommandAllocator(_Type, IID_PPV_ARGS(mCommandAllocator.GetAddressOf()))), "CreateCommandAllocator is failed.");
	}

	void DeviceContext::CreateCommandList(ID3D12Device* _Device, D3D12_COMMAND_LIST_TYPE _Type)
	{
		_ASSERT_EXPR_A(SUCCEEDED(_Device->CreateCommandList(0, _Type, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(mCommandList.GetAddressOf()))), "CreateCommandList is failed.");
		mCommandList->Close();
	}

	void DeviceContext::CreateSwapChain(HWND _Hwnd, IDXGIFactory6* _Factory)
	{
		constexpr DXGI_SWAP_CHAIN_DESC1 lSwapChainDesc{
			.Width = Window::kSize.x,
			.Height = Window::kSize.y,
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
		_ASSERT_EXPR_A(SUCCEEDED(_Factory->CreateSwapChainForHwnd(mCommandQueue.Get(), _Hwnd, &lSwapChainDesc, nullptr, nullptr, lSwapChain1.GetAddressOf())), "CreateSwapChainForHwnd is failed.");
		lSwapChain1.As(&mSwapChain4);
	}

	void DeviceContext::CreateRenderTarget(ID3D12Device* _Device)
	{
		constexpr D3D12_DESCRIPTOR_HEAP_DESC lRTHeapDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			.NumDescriptors = kBackBufferNum,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			.NodeMask = 0
		};
		_ASSERT_EXPR_A(SUCCEEDED(_Device->CreateDescriptorHeap(&lRTHeapDesc, IID_PPV_ARGS(mRT.mDescriptorHeap.GetAddressOf()))), "CreateDescriptorHeap is failed.");
		mRT.mDescriptorSize = _Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE lCpuHandle(mRT.mDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		constexpr D3D12_RENDER_TARGET_VIEW_DESC lRTVDesc{
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
		};
		for (uint i = 0; i < kBackBufferNum; ++i)
		{
			_ASSERT_EXPR_A(SUCCEEDED(mSwapChain4->GetBuffer(i, IID_PPV_ARGS(mRT.mBuffers[i].GetAddressOf()))), "GetBuffer is failed.");
			_Device->CreateRenderTargetView(mRT.mBuffers[i].Get(), &lRTVDesc, lCpuHandle);
			lCpuHandle.Offset(mRT.mDescriptorSize);
		}
	}

	void DeviceContext::CreateDepthStencil(ID3D12Device* _Device)
	{
		constexpr D3D12_DESCRIPTOR_HEAP_DESC lDSHeapDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
			.NumDescriptors = 1,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			.NodeMask = 0
		};
		_ASSERT_EXPR_A(SUCCEEDED(_Device->CreateDescriptorHeap(&lDSHeapDesc, IID_PPV_ARGS(mDS.mDescriptorHeap.GetAddressOf()))), "CreateDescriptorHeap is failed.");
		CD3DX12_CPU_DESCRIPTOR_HANDLE lCpuHandle(mDS.mDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		constexpr D3D12_RESOURCE_DESC lResourceDesc{
			.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			.Width = Window::kSize.x,
			.Height = Window::kSize.y,
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
		_ASSERT_EXPR_A(SUCCEEDED(_Device->CreateCommittedResource(&lHeapProperties, D3D12_HEAP_FLAG_NONE, &lResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &lClearValue, IID_PPV_ARGS(mDS.mBuffer.GetAddressOf()))), "CreateCommittedResource is failed.");
		constexpr D3D12_DEPTH_STENCIL_VIEW_DESC lDSVDesc{
			.Format = DXGI_FORMAT_D32_FLOAT,
			.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
			.Flags = D3D12_DSV_FLAG_NONE,
			.Texture2D = {.MipSlice = 0 }
		};
		_Device->CreateDepthStencilView(mDS.mBuffer.Get(), &lDSVDesc, lCpuHandle);
	}

	void DeviceContext::CreateFence(ID3D12Device* _Device)
	{
		_ASSERT_EXPR_A(SUCCEEDED(_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mSync.mFence.GetAddressOf()))), "CreateFence is failed.");
		mSync.mEvent = CreateEvent(nullptr, false, false, nullptr);
		mSync.mValue = 0;
	}
}