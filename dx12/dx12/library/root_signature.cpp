#include "root_signature.h"
#include "utility.h"

using namespace Microsoft::WRL;
namespace snd
{
	void RootSignature::Create(ID3D12Device* _device, uint _num_param, const CD3DX12_ROOT_PARAMETER* _param, uint _num_sampler, const D3D12_STATIC_SAMPLER_DESC* _sampler, const D3D12_ROOT_SIGNATURE_FLAGS _flags)
	{
		ComPtr<ID3DBlob> error;
		ComPtr<ID3DBlob> root_signature_blob;

		root_signature_desc_.Init(_num_param, _param, _num_sampler, _sampler, _flags);
		ASSERT_SUCCEEDED(D3D12SerializeRootSignature(&root_signature_desc_, D3D_ROOT_SIGNATURE_VERSION_1_0, root_signature_blob.GetAddressOf(), error.GetAddressOf()));
		
		ASSERT_SUCCEEDED(_device->CreateRootSignature(0, root_signature_blob->GetBufferPointer(), root_signature_blob->GetBufferSize(), IID_PPV_ARGS(&root_signature_)));
	}
}