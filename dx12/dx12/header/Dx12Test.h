#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>

#include "constants.h"

class Dx12Test
{
public:
	Dx12Test() = default;
	~Dx12Test() = default;
public:
	void Initialize(HWND _hwnd);
	void RenderBegin();
	void RenderEnd();
	void WaitFence();
private:
	Microsoft::WRL::ComPtr<IDXGIFactory6> factory6;
	Microsoft::WRL::ComPtr<ID3D12Device5> device5;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swap_chain4;
	UINT frame_buffer_index = 0;

	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissor_rect = {};

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command_list;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kFrameBufferCount> back_buffer;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTV_heap;
	UINT RTV_heap_size;

	Microsoft::WRL::ComPtr<ID3D12Resource> depth_buffer;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DSV_heap;

	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fence_value;
	HANDLE fence_event;
};