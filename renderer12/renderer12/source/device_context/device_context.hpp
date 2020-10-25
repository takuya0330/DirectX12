#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <functional>

namespace re12::detail
{
	class DeviceContext
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		template<class T, UINT _Num> using ArrayComPtr = std::array<ComPtr<T>, _Num>;
	private:
		ComPtr<IDXGISwapChain4> mSwapChain4;
	private:
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;
	private:
		static constexpr UINT kBackBufferNum = 3;
		UINT mBackBufferIndex = 0;
		UINT mBackBufferHeapSize = 0;
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
		D3D12_VIEWPORT mViewport = {};
		D3D12_RECT mScissorRect = {};
	private:
		ComPtr<ID3D12DescriptorHeap> mImGuiHeap;
	public:
		DeviceContext() = default;
		~DeviceContext();
	public:
		void Initialize();
		void ImGuiUpdate();
		void Reset();
		void BarrierTransition(D3D12_RESOURCE_STATES _Before, D3D12_RESOURCE_STATES _Affter);
		void ClearScreen(const float* _ClearColor);
		void SetViewport(UINT _Width, UINT _Height);
		void ImGuiRender();
		void Execute();
		void ImGuiConfig();
		void Present(UINT _Vsync = 0);
		void WaitFence();
	public:
		void ImGuiFunction(const char* _Title, std::function<void()> _Function);
	public:
		//ID3D12CommandQueue* GetCommandQueue()const { return mCommandQueue.Get(); }
		//ID3D12CommandAllocator* GetCommandAllocator()const { return mCommandAllocator.Get(); }
		ID3D12GraphicsCommandList* GetCommandList()const { return mCommandList.Get(); }
	};
}