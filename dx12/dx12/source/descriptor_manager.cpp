#include "../header/descriptor_manager.h"
#include "../header/utility.h"

DescriptorManager::DescriptorManager(ID3D12Device* _device, const D3D12_DESCRIPTOR_HEAP_DESC& _desc)
{
	ASSERT_SUCCEEDED(_device->CreateDescriptorHeap(&_desc, IID_PPV_ARGS(&descriptor_heap)));
	descriptor_handle = DescriptorHandle(descriptor_heap->GetCPUDescriptorHandleForHeapStart(), descriptor_heap->GetGPUDescriptorHandleForHeapStart());
	index = 0;
	descriptor_size = _device->GetDescriptorHandleIncrementSize(_desc.Type);
}

void DescriptorManager::Free(const DescriptorHandle& _handle)
{
	free_list.push_back(_handle);
}

DescriptorHandle DescriptorManager::Allocate()
{
	if (!free_list.empty())
	{
		DescriptorHandle front = free_list.front();
		free_list.pop_front();
		return front;
	}

	UINT i = index++;
	return DescriptorHandle(descriptor_handle.CPUOffset(i, descriptor_size), descriptor_handle.GPUOffset(i, descriptor_size));
}
