#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "external/d3dx12/d3dx12.h"

namespace snd::detail
{
	class resource
	{
	public:
		resource() {}
		~resource() {}

		void create(ID3D12Device* _device, 
			const D3D12_HEAP_TYPE _type, 
			const D3D12_HEAP_FLAGS _flags, 
			const D3D12_RESOURCE_DESC* _desc, 
			D3D12_RESOURCE_STATES _states, 
			const D3D12_CLEAR_VALUE* _clear_value = nullptr);
		void create(IDXGISwapChain4* _swap_chain, UINT _buffer_count);

		ID3D12Resource* get()const { return buffer_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
	};
}