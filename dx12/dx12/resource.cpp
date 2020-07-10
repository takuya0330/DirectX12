#include "resource.h"
#include "utility.h"

namespace snd::detail
{
	void resource::create(
		ID3D12Device* _device, 
		const D3D12_HEAP_TYPE _type, 
		const D3D12_HEAP_FLAGS _flags, 
		const D3D12_RESOURCE_DESC* _desc, 
		D3D12_RESOURCE_STATES _states, 
		const D3D12_CLEAR_VALUE* _clear_value)
	{
		ASSERT_SUCCEEDED(_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(_type),
			_flags,
			_desc,
			_states,
			_clear_value,
			IID_PPV_ARGS(&buffer_)));
		utility::print("ID3D12Resource create successed.");
	}

	void resource::create(IDXGISwapChain4* _swap_chain, UINT _buffer_count)
	{
		ASSERT_SUCCEEDED(_swap_chain->GetBuffer(_buffer_count, IID_PPV_ARGS(&buffer_)));
		utility::print("ID3D12Resource(%d) create successed.", _buffer_count);
	}
}