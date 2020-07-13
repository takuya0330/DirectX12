#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <array>

#include "constants.h"
#include "math.h"

namespace snd::detail
{
	class Renderer
	{
	public:
		Renderer() {}
		~Renderer() {}

		void Create(HWND _hwnd, ID3D12Device* _device, IDXGIFactory6* _factory);

		void Reset();
		void Barrier(D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _affter);
		void Clear(const float* _clear_color);
		void SetViewport(uint2 _Resolution);
		void Present(uint _sync = 1);
		void WaitGPU();
	private:
		uint current_back_buffer_;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> swap_chain_;

		struct Command
		{
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue_;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator_;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> list_;
		} command_;

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
	};
}