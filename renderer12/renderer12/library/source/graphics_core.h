#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <iostream>
#include <array>

#include "constants.h"
#include "math.h"

namespace snd
{
	//class VertexBuffer;
	//class IndexBuffer;
	//class PipelineStateObject;

	class D3D12Core
	{
	public:
		D3D12Core() {}
		~D3D12Core() {}

		void Initialize(HWND _hwnd);

		void Reset();

		void Barrier(D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _affter);

		void Clear(const float* _clear_color);

		void SetViewport(uint2 _resolution);

		void Present(uint _sync = 0);

		void WaitForCommandQueue();

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateGraphicsCommandList(const D3D12_COMMAND_LIST_TYPE _type, ID3D12CommandAllocator* _allocator);

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE _type, uint _num, bool _access_shader = false);

		Microsoft::WRL::ComPtr<ID3D12Resource> CreateResource(const D3D12_HEAP_TYPE _type, uint _size, const void* _data);

		//VertexBuffer CreateVertexBuffer(const void* _data, uint _size, uint _stride);

		//IndexBuffer CreateIndexBuffer(const void* _data, uint _size);

		//PipelineStateObject CreatePipelineStateObject();
	private:
		Microsoft::WRL::ComPtr<ID3D12Device> device_;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> swap_chain_;
		uint current_back_buffer_index_;

		struct CommandDevice
		{
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue_;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator_;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> list_;
		} command_device_;

		struct RenderTarget
		{
			uint increment_size_;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
			std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, constants::kBackBufferCount> buffers_;
		} render_target_;

		struct DepthStencil
		{
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
			Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
		} depth_stencil_;

		struct Sync
		{
			HANDLE event_;
			UINT64 value_;
			Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
		} sync_;

		void CreateDebugLayer();

		void CreateInfoQueue();

		void CreateDevice(IDXGIAdapter1* _adapter, const D3D_FEATURE_LEVEL _feature_level);
		
		void CreateSwapChain(IDXGIFactory4* _factory, HWND _hwnd);

		void CreateRenderTarget();

		void CreateDepthStencil();

		void CreateFence();

		Microsoft::WRL::ComPtr<IDXGIFactory4> CreateFactory(uint _dxgi_flags);

		Microsoft::WRL::ComPtr<IDXGIAdapter1> CreateAdapter(IDXGIFactory4* _factory, const D3D_FEATURE_LEVEL _feature_level);

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE _type);

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE _type);
	};
}