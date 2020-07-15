#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>

#include "../external/d3dx12/d3dx12.h"

namespace snd
{
	class RootSignature
	{
	public:
		RootSignature() {}
		~RootSignature() {}

		ID3D12RootSignature* Get()const { return root_signature_.Get(); }
		ID3D12RootSignature* operator->()const { return root_signature_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> root_signature_;
	};
}