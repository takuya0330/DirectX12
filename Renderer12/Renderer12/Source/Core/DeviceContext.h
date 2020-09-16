#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>

#include "../Mathematics.h"

namespace gl
{
	class DeviceContext
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		template<class T, uint _Num> using ArrayComPtr = std::array<ComPtr<T>, _Num>;
	private:
		ComPtr<IDXGISwapChain4> mSwapChain4;
	private:
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;
	private:
		static constexpr uint kBackBufferNum = 3;
		uint mBackBufferIndex = 0;
		uint mBackBufferHeapSize = 0;
		ComPtr<ID3D12DescriptorHeap> mBackBufferHeap;
		ArrayComPtr<ID3D12Resource, kBackBufferNum> mBackBuffers;
	private:
		ComPtr<ID3D12DescriptorHeap> mDepthStencilHeap;
		ComPtr<ID3D12Resource> mDepthBuffer;
	private:
		ComPtr<ID3D12Fence> mFence;
		UINT64 mFenceValue = 0;
		HANDLE mFenceEvent = 0;
	private:
		D3D12_VIEWPORT mViewport;
		D3D12_RECT mScissorRect;
	public:
		static constexpr bool mUseImGui = true;
	private:
		ComPtr<ID3D12DescriptorHeap> mImGuiHeap;
	public:
		DeviceContext() = default;
		~DeviceContext();
	public:
		void Initialize();
		void NewFrame();
		void Reset();
		void BarrierTransition(D3D12_RESOURCE_STATES _Before, D3D12_RESOURCE_STATES _Affter);
		void ClearScreen(const float* _ClearColor);
		void SetViewport(const uint2& _WindowSize);
		void GUIRender();
		void Execute();
		void GUIConfig();
		void Present(uint _Vsync = 0);
		void WaitFence();
	public:
		//ID3D12CommandQueue* GetCommandQueue()const { return mCommandQueue.Get(); }
		//ID3D12CommandAllocator* GetCommandAllocator()const { return mCommandAllocator.Get(); }
		//ID3D12GraphicsCommandList* GetCommandList()const { return mCommandList.Get(); }
	};
}