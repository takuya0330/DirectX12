#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <memory>

#include "constants.h"
#include "descriptor_manager.h"

class D3D12Manager
{
public:
	D3D12Manager() = default;
	~D3D12Manager() = default;
public:
	void Initialize(HWND _hwnd);
	void RenderBegin();
	void RenderEnd();
	void BarrierTranslation(ID3D12Resource* _resource, D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _after);
	void ScreenClear();
	void SetViewport();
	void WaitFence();
	void WaitIdle();
private:
	void CreateDebugLayer();
	void CreateFactory(UINT _dxgiflags);
	void CreateDevice(UINT _dxgiflags);
	void CreateInfoQueue();
	void CreateCommandBuffer();
	void CreateSwapChain(HWND _hwnd);
	void CreateDescriptorHeap();
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void CreateFence();
	void SetupViewport();
private:
	Microsoft::WRL::ComPtr<IDXGIFactory6> factory6;
	Microsoft::WRL::ComPtr<ID3D12Device5> device5;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swap_chain4;
	UINT frame_buffer_index = 0;

	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissor_rect = {};

	struct
	{
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue;
		std::array<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, kFrameBufferCount> allocators;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> list;
	} command_buffer;

	struct
	{
		std::shared_ptr<DescriptorManager> RTV;
		std::shared_ptr<DescriptorManager> DSV;
		std::shared_ptr<DescriptorManager> CBV_SRV_UAV;
	} descriptor_heap;

	struct
	{
		std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kFrameBufferCount> buffer;
		std::array<DescriptorHandle, kFrameBufferCount> handle;
	} RTV;

	struct
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
		DescriptorHandle handle;
	} DSV;

	struct
	{
		HANDLE fence_event;
		Microsoft::WRL::ComPtr<ID3D12Fence> fence;
		std::array<UINT64, kFrameBufferCount> fence_value;
		std::array<Microsoft::WRL::ComPtr<ID3D12Fence>, kFrameBufferCount> fences;
	} sync;
};