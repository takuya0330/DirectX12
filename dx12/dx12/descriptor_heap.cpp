#include "descriptor_heap.h"
#include "utility.h"

namespace snd::detail
{
	void descriptor_heap::create(ID3D12Device* _device, const D3D12_DESCRIPTOR_HEAP_TYPE _type, UINT _num)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		{
			desc.Type = _type;									// 作成するビューの指定
			desc.NumDescriptors = _num;						// 表裏の2つ
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;// 特になし
			desc.NodeMask = 0;									// 複数のGPUがある場合に識別するビットフラグ 
		}
		ASSERT_SUCCEEDED(_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptor_heap_)));
		increment_size_ = _device->GetDescriptorHandleIncrementSize(_type);
		cpu_handle_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptor_heap_->GetCPUDescriptorHandleForHeapStart());
		gpu_handle_ = CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptor_heap_->GetGPUDescriptorHandleForHeapStart());
		utility::print("ID3D12DescriptorHeap create successed.");
	}

	void descriptor_heap::cpu_offset(int num)
	{
		cpu_handle_.Offset(num, increment_size_);
	}
}