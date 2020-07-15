#include "resource.h"
#include "utility.h"

namespace snd::detail
{
	void Resource::Create(ID3D12Device* _device, uint _buffer_size, const void* _init_data)
	{
		ASSERT_SUCCEEDED(_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 
			D3D12_HEAP_FLAG_NONE, 
			&CD3DX12_RESOURCE_DESC::Buffer(_buffer_size),
			D3D12_RESOURCE_STATE_GENERIC_READ, 
			nullptr, 
			IID_PPV_ARGS(&buffer_)));

		if (_init_data)
		{
			ASSERT_SUCCEEDED(buffer_->Map(0, nullptr, &data_));
			memcpy(data_, _init_data, _buffer_size);
			buffer_->Unmap(0, nullptr);
		}
	}
}