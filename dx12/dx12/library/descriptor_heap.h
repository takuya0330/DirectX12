#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>

#include "../external/d3dx12/d3dx12.h"

namespace snd
{
	class DescriptorHeap
	{
	public:
		DescriptorHeap() {}
		~DescriptorHeap() {}

		ID3D12DescriptorHeap* Get()const { return descriptor_heap_.Get(); }
		UINT GetIncrementSize()const { return increment_size_; }
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandle()const { return cpu_handle_; }
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandle()const { return gpu_handle_; }
	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
		UINT increment_size_;
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
	};
}