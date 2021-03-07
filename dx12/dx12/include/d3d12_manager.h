#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>

#include "property.h"

constexpr UINT kFrameBufferCount = 2;

using namespace Microsoft::WRL;

class D3D12Manager
{
public:
	D3D12Manager() = default;
	~D3D12Manager() = default;

	bool Initialize(HWND _hwnd, UINT _width, UINT _height);
	void SetFrameIndex();
	void Reset();
	void BarrierRenderTarget();
	void ClearRenderTargetView(const float* _color);
	void ClearDepthStencilView();
	void SetRenderTargets();
	void SetViewport(UINT _width, UINT _height);
	void BarrierPresent();
	void ExecuteCommandQueue();
	void Present(UINT _vsync);
	void WaitPreviousFrame();

	Property<ID3D12Device*> Device{ [this]() { return device_.Get(); }, nullptr };
	Property<ID3D12GraphicsCommandList*> GraphicsCommandList{ [this]() { return command_list_.Get(); }, nullptr };
private:
	bool CreateDebugLayer();
	bool CreateFactory(UINT _dxgiflags);
	bool CreateDevice();
	bool CreateInfoQueue();
	bool CreateCommandQueue(D3D12_COMMAND_LIST_TYPE _type);
	bool CreateCommandAllocators(D3D12_COMMAND_LIST_TYPE _type);
	bool CreateCommandList(D3D12_COMMAND_LIST_TYPE _type);
	bool CreateSwapChain(HWND _hwnd, UINT _width, UINT _height);
	bool CreateRenderTarget();
	bool CreateDepthStencil(UINT _width, UINT _height);
	bool CreateFences();

	ComPtr<IDXGIFactory6> factory_;
	ComPtr<ID3D12Device5> device_;
	ComPtr<IDXGISwapChain4> swap_chain_;
	UINT frame_index_ = 0;

	ComPtr<ID3D12CommandQueue> command_queue_;
	std::array<ComPtr<ID3D12CommandAllocator>, kFrameBufferCount> command_allocators_;
	ComPtr<ID3D12GraphicsCommandList> command_list_;

	std::array<ComPtr<ID3D12DescriptorHeap>, kFrameBufferCount> render_target_heaps_;
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, kFrameBufferCount> render_target_cpu_handles_ = {};
	std::array<ComPtr<ID3D12Resource>, kFrameBufferCount> frame_buffers_;

	ComPtr<ID3D12DescriptorHeap> depth_stencil_heap_;
	D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil_cpu_handle_ = {};
	ComPtr<ID3D12Resource> depth_buffer_;

	std::array<ComPtr<ID3D12Fence>, kFrameBufferCount> fences_;
	std::array<UINT64, kFrameBufferCount> fence_values_ = {};
	HANDLE fence_event_ = {};

	D3D12_VIEWPORT viewport_ = {};
	D3D12_RECT scissor_rect_ = {};
};