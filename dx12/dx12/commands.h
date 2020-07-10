#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>


namespace snd::detail
{
	class commands
	{
	public:
		commands() {}
		~commands() {}

		void create(ID3D12Device* _device);

		ID3D12CommandQueue* GetQueue()const { return command_queue_.Get(); }
		ID3D12CommandAllocator* GetAllocator()const { return command_allocator_.Get(); }
		ID3D12GraphicsCommandList* GetList()const { return graphics_command_list_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queue_;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator_;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> graphics_command_list_;
	};
}