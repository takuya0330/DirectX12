#include "../external/d3dx12/d3dx12.h"

#include "buffer.h"
#include "utility.h"

namespace snd::detail
{
	void Resource::Create(ID3D12Device* _device, uint _size)
	{
		ASSERT_MESSAGE(_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(_size),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&resource_)),
			"CreateCommittedResource() failed.");
	}

	void Resource::Mapping(const void* _data, size_t _size)
	{
		assert(resource_);

		void* mapped = nullptr;
		CD3DX12_RANGE range(0, 0);
		ASSERT_MESSAGE(resource_->Map(0, &range, &mapped), "ID3D12Resource::Map() failed.");
		memcpy(mapped, _data, _size);
		resource_->Unmap(0, nullptr);
	}
}

namespace snd
{
	void VertexBuffer::Setup(uint _size, uint _stride)
	{
		view_.BufferLocation = resource_->GetGPUVirtualAddress();
		view_.SizeInBytes = _size;											
		view_.StrideInBytes = _stride;									
	}

	void IndexBuffer::Setup(uint _size)
	{
		view_.BufferLocation = resource_->GetGPUVirtualAddress();
		view_.SizeInBytes = _size;											
		view_.Format = DXGI_FORMAT_R32_UINT;
	}
}