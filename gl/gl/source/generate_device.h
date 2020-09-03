#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

namespace gl
{
	class generate_device
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		ComPtr<ID3D12Device5> device_;
		ComPtr<IDXGIFactory6> factory_;
		ComPtr<ID3D12CommandQueue> command_queue_;
		ComPtr<ID3D12CommandAllocator> command_allocator_;
		ComPtr<ID3D12GraphicsCommandList> command_list_;
		bool use_wrap_adapter_ = false;

	private:
		void create_debug_layer();
		void create_info_queue();
		void create_factory(UINT _dxgi_flags);
		void create_device(IDXGIAdapter4* _adapter, const D3D_FEATURE_LEVEL _feature_level);
		void create_command_queue(D3D12_COMMAND_LIST_TYPE _command_list_type);
		void create_command_allocator(D3D12_COMMAND_LIST_TYPE _command_list_type);
		void create_command_list(D3D12_COMMAND_LIST_TYPE _command_list_type);

		ComPtr<IDXGIAdapter4> search_adapter(IDXGIFactory6* _factory, const D3D_FEATURE_LEVEL _feature_level);

	public:
		generate_device() = default;
		~generate_device() = default;

		void initialize();
		void reset();
		void barrier_transition(ID3D12Resource* _resource, D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _affter);
		void close();

		ID3D12Device5* get_device()const { return device_.Get(); }
		IDXGIFactory6* get_factory()const { return factory_.Get(); }
		ID3D12CommandQueue* get_command_queue()const { return command_queue_.Get(); }
		ID3D12CommandAllocator* get_command_allocator()const { return command_allocator_.Get(); }
		ID3D12GraphicsCommandList* get_command_list()const { return command_list_.Get(); }
	};
}