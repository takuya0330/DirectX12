#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "../external/d3dx12/d3dx12.h"
#include "math.h"

namespace snd::detail
{
	class Resource
	{
	public:
		Resource() {}
		~Resource() {}

		void Create(ID3D12Device* _device, uint _buffer_size, const void* _init_data);
		ID3D12Resource* Get()const { return buffer_.Get(); }
		ID3D12Resource* operator->()const { return buffer_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
		void* data_;
	};
}
