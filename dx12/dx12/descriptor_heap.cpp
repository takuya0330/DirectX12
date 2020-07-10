#include "descriptor_heap.h"
#include "utility.h"

namespace snd::detail
{
	void descriptor_heap::create(ID3D12Device* _device, const D3D12_DESCRIPTOR_HEAP_TYPE _type, UINT _num)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		{
			desc.Type = _type;									// �쐬����r���[�̎w��
			desc.NumDescriptors = _num;						// �\����2��
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;// ���ɂȂ�
			desc.NodeMask = 0;									// ������GPU������ꍇ�Ɏ��ʂ���r�b�g�t���O 
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