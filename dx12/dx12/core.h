#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <array>

#include "external/d3dx12/d3dx12.h"

#include "commands.h"
#include "descriptor_heap.h"
#include "resource.h"

namespace snd::detail
{
	class core
	{
	public:
		core() {}
		~core() {}

	public:
		void initialize(HWND _hwnd, UINT _width, UINT _height);
		void uninitialize();

	public:
		Microsoft::WRL::ComPtr<ID3D12Device> get_device()const { return device_; }

	private:
		Microsoft::WRL::ComPtr<ID3D12Device> device_;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> swap_chain_;
		
		commands commands_;

		struct render_target
		{
			static constexpr UINT kBufferCount = 2;
			descriptor_heap descriptor_heap_;
			std::array<resource, kBufferCount> buffers_;
		} render_target_;

		struct depth_stencil
		{
			descriptor_heap descriptor_heap_;
			resource buffer_;
		} depth_stencil_;

		struct sync
		{
			HANDLE event_;
			UINT64 value_;
			Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
		} sync_;

		/*struct RenderTarget
		{
			static constexpr UINT kFrameCount = 3;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
			std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kFrameCount> resources_;
			UINT descriptor_size_;
		} render_target_;

		struct DepthStencil
		{
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
			Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
		} depth_stencil_;

		HANDLE hfence_;
		UINT64 fence_value_;
		Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

		CD3DX12_VIEWPORT viewport_;
		CD3DX12_RECT scissor_;*/
	};
}