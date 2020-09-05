#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>

#include "Mathematics.h"

namespace gl
{
	// ToDo. 
	// ComputeShader‘Î‰ž
	// WindowMode <-> Fullscreen‘Î‰ž 
	class DeviceContext
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	public:
		static constexpr uint kBackBufferNum = 3;
	private:
		uint mBackBufferIndex = 0;
		ComPtr<IDXGISwapChain4> mSwapChain4;
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;
		D3D12_VIEWPORT mViewport;
		D3D12_RECT mScissorRect;
	private:
		struct RenderTarget
		{
			std::array<ComPtr<ID3D12Resource>, kBackBufferNum> mBuffers;
			ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;
			uint mDescriptorSize = 0;
		} mRT;
	private:
		struct DepthStencil
		{
			ComPtr<ID3D12Resource> mBuffer;
			ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;
		} mDS;
	private:
		struct Sync
		{
			ComPtr<ID3D12Fence> mFence;
			UINT64 mValue = 0;
			HANDLE mEvent = 0;
		} mSync;
	private:
		void CreateCommandQueue(ID3D12Device* _Device, D3D12_COMMAND_LIST_TYPE _Type);
		void CreateCommandAllocator(ID3D12Device* _Device, D3D12_COMMAND_LIST_TYPE _Type);
		void CreateCommandList(ID3D12Device* _Device, D3D12_COMMAND_LIST_TYPE _Type);
		void CreateSwapChain(HWND _Hwnd, IDXGIFactory6* _Factory);
		void CreateRenderTarget(ID3D12Device* _Device);
		void CreateDepthStencil(ID3D12Device* _Device);
		void CreateFence(ID3D12Device* _Device);
	public:
		DeviceContext() = default;
		~DeviceContext() = default;
	public:
		void Initialize();
		void Reset();
		void SetBarrier(D3D12_RESOURCE_STATES _Before, D3D12_RESOURCE_STATES _Affter);
		void ClearScreen(const float* _ClearColor);
		void SetViewport(const uint2& _WindowSize);
		void Execute();
		void Present(uint _Vsync = 0);
		void WaitGpu();
	public:
		ID3D12CommandQueue* GetCommandQueue()const { return mCommandQueue.Get(); }
		ID3D12CommandAllocator* GetCommandAllocator()const { return mCommandAllocator.Get(); }
		ID3D12GraphicsCommandList* GetCommandList()const { return mCommandList.Get(); }
	};
}