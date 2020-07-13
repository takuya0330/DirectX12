#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "../external/d3dx12/d3dx12.h"

namespace snd
{
	class Resource
	{
	public:
		Resource() {}
		~Resource() {}

		ID3D12Resource* Get()const { return buffer_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
		void* data_;
	};
}