#include "../include/pipeline_manager.h"
#include "../include/d3d12_helper.h"
#include "../include/utility.h"

bool PipelineManager::Initialize(
	const std::string& _key,
	ID3D12Device* _device,
	UINT _parameter_num,
	const D3D12_ROOT_PARAMETER* _parameters,
	UINT _static_sampler_num,
	const D3D12_STATIC_SAMPLER_DESC* _static_samplers,
	const D3D12_SHADER_BYTECODE& _vs,
	const D3D12_SHADER_BYTECODE& _ps,
	const D3D12_SHADER_BYTECODE& _ds,
	const D3D12_SHADER_BYTECODE& _hs,
	const D3D12_SHADER_BYTECODE& _gs,
	const D3D12_BLEND_DESC& _blend_desc,
	const D3D12_RASTERIZER_DESC& _rasterizer_desc,
	const D3D12_DEPTH_STENCIL_DESC& _depth_stencil_desc,
	const D3D12_INPUT_LAYOUT_DESC& _input_layout_desc)
{
	const auto& root_signature_key = root_signatures_.find(_key);
	const auto& pipeline_state_key = pipeline_states_.find(_key);
	if ((root_signature_key != root_signatures_.end()) && (pipeline_state_key != pipeline_states_.end()))
	{
		debug::Console(std::string(_key + " : already created.\n").c_str());
		return true;
	}

	// ルートシグネチャの作成
	D3D12_ROOT_SIGNATURE_DESC root_signature_desc = cd3d12::CreateRootSignatureDesc(
		_parameter_num, _parameters, _static_sampler_num, _static_samplers);
	ComPtr<ID3DBlob> blob, err;
	auto hr = D3D12SerializeRootSignature(
		&root_signature_desc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		blob.GetAddressOf(),
		err.GetAddressOf());
	if (FAILED(hr))
	{
		debug::Console("Failed : D3D12SerializeRootSignature\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return false;
	}

	ComPtr<ID3D12RootSignature> root_signature;
	hr = _device->CreateRootSignature(
		0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&root_signature));
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateRootSignature\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return false;
	}
	root_signatures_.insert(std::make_pair(_key, root_signature));

	// パイプラインステートの作成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc = cd3d12::CreatePipelineStateDesc(
		root_signature.Get(), _vs, _ps, _ds, _hs, _gs, _blend_desc, _rasterizer_desc, _depth_stencil_desc, _input_layout_desc);
	ComPtr<ID3D12PipelineState> pipeline_state;
	hr = _device->CreateGraphicsPipelineState(&pipeline_state_desc, IID_PPV_ARGS(&pipeline_state));
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateGraphicsPipelineState\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return false;
	}
	pipeline_states_.insert(std::make_pair(_key, pipeline_state));

	return true;
}

void PipelineManager::Bind(ID3D12GraphicsCommandList* _command_list, const std::string& _key)
{
	_command_list->SetPipelineState(pipeline_states_.at(_key).Get());
	_command_list->SetGraphicsRootSignature(root_signatures_.at(_key).Get());
}
