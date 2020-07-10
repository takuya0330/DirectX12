#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>

#include "external/d3dx12/d3dx12.h"

namespace snd::detail
{
	class descriptor_heap
	{
	public:
		descriptor_heap() {}
		~descriptor_heap() {}

		void create(ID3D12Device* _device, const D3D12_DESCRIPTOR_HEAP_TYPE _type, UINT _num);
		void cpu_offset(int num);

		ID3D12DescriptorHeap* get()const { return descriptor_heap_.Get(); }
		UINT get_increment_size()const { return increment_size_; }
		CD3DX12_CPU_DESCRIPTOR_HANDLE get_cpu_handle()const { return cpu_handle_; }
		CD3DX12_GPU_DESCRIPTOR_HANDLE get_gpu_handle()const { return gpu_handle_; }
	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
		UINT increment_size_;
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
	};
}