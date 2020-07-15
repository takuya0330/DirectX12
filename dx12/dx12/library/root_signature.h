#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>

#include "../external/d3dx12/d3dx12.h"

#include "math.h"

namespace snd
{
	class RootSignature
	{
	public:
		RootSignature() {}
		~RootSignature() {}

		void Create(ID3D12Device* _device, 
			uint _num_param, const CD3DX12_ROOT_PARAMETER* _param, 
			uint _num_sampler, const D3D12_STATIC_SAMPLER_DESC* _sampler, const D3D12_ROOT_SIGNATURE_FLAGS _flags);

		ID3D12RootSignature* Get()const { return root_signature_.Get(); }
		ID3D12RootSignature* operator->()const { return root_signature_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> root_signature_;
		CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc_;
	};
}