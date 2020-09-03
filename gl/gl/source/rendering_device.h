#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>

#include "../third_party/d3dx12/d3dx12.h"

#include "constants.h"

namespace gl
{
	class rendering_device
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		ComPtr<IDXGISwapChain4> swap_chain_;
		UINT frame_buffer_index_ = 0;

		ComPtr<ID3D12CommandQueue> command_queue_;
		ComPtr<ID3D12CommandAllocator> command_allocator_;
		ComPtr<ID3D12GraphicsCommandList> command_list_;

		ComPtr<ID3D12DescriptorHeap> gui_descriptor_heap_;

		CD3DX12_VIEWPORT viewport_;
		CD3DX12_RECT scissor_rect_;

		struct
		{
			std::array<ComPtr<ID3D12Resource>, kFrameBufferNum> buffers_;
			ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
			uint increment_size_;
		} render_target_;

		struct
		{
			ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
			ComPtr<ID3D12Resource> buffer_;
		} depth_stencil_;

		struct
		{
			ComPtr<ID3D12Fence> fence_;
			UINT64 value_;
			HANDLE event_;
		} sync_;

	private:
		void create_swap_chain(HWND _hwnd, IDXGIFactory6* _factory);
		void create_command_queue(ID3D12Device* _device, D3D12_COMMAND_LIST_TYPE _command_list_type);
		void create_command_allocator(ID3D12Device* _device, D3D12_COMMAND_LIST_TYPE _command_list_type);
		void create_command_list(ID3D12Device* _device, D3D12_COMMAND_LIST_TYPE _command_list_type);
		void create_render_target(ID3D12Device* _device);
		void create_depth_stencil(ID3D12Device* _device);
		void create_fence(ID3D12Device* _device);
		void create_gui_descritor_heap(ID3D12Device* _device);

	public:
		rendering_device() = default;
		~rendering_device() = default;

		void initialize(HWND _hwnd);
		void reset();
		void barrier_transition(D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _affter);
		void screen_clear(const float* _clear_color);
		void set_viewport(const uint2& _window_size);
		void execute();
		void present(uint _sync = 0);
		void wait_previous_frame();
		void set_gui_descriptor_heap();

		ID3D12CommandQueue* get_command_queue()const { return command_queue_.Get(); }
		ID3D12CommandAllocator* get_command_allocator()const { return command_allocator_.Get(); }
		ID3D12GraphicsCommandList* get_command_list()const { return command_list_.Get(); }
		ID3D12DescriptorHeap* get_gui_descriptor_heap()const { return gui_descriptor_heap_.Get(); }
	};
}