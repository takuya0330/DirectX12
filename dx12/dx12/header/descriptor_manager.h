#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <wrl.h>
#include <list>

class DescriptorHandle
{
public:
	DescriptorHandle() = default;
	~DescriptorHandle() = default;
	DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE _cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE _gpu_handle) : cpu_handle(_cpu_handle), gpu_handle(_gpu_handle) {}
public:
	D3D12_CPU_DESCRIPTOR_HANDLE& CPUOffset(size_t _index, size_t _size) { cpu_handle.ptr += _index * _size; return cpu_handle; }
	D3D12_GPU_DESCRIPTOR_HANDLE& GPUOffset(size_t _index, size_t _size) { gpu_handle.ptr += _index * _size; return gpu_handle; }
public:
	operator D3D12_CPU_DESCRIPTOR_HANDLE() const { return cpu_handle; }
	operator D3D12_GPU_DESCRIPTOR_HANDLE() const { return gpu_handle; }
private:
	D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle;
};

class DescriptorManager
{
public:
	DescriptorManager(ID3D12Device* _device, const D3D12_DESCRIPTOR_HEAP_DESC& _desc);
	~DescriptorManager() = default;
public:
	void Free(const DescriptorHandle& _handle);
	DescriptorHandle Allocate();
public:
	ID3D12DescriptorHeap* Get() { return descriptor_heap.Get(); }
	ID3D12DescriptorHeap* const* GetAddress() { return descriptor_heap.GetAddressOf(); }
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap;
	DescriptorHandle descriptor_handle;
	UINT index;
	UINT descriptor_size;
	std::list<DescriptorHandle> free_list;
};