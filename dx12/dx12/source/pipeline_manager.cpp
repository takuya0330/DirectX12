#include "../include/pipeline_manager.h"
#include "../include/utility.h"

bool PipelineManager::Initialize(
	const std::string& _key,
	ID3D12Device* _device,
	const D3D12_ROOT_PARAMETER* _root_params,
	UINT _root_param_num,
	const D3D12_STATIC_SAMPLER_DESC* _static_samplers,
	UINT _static_sampler_num,
	const D3D12_SHADER_BYTECODE& _vs,
	const D3D12_SHADER_BYTECODE& _hs,
	const D3D12_SHADER_BYTECODE& _ds,
	const D3D12_SHADER_BYTECODE& _gs,
	const D3D12_SHADER_BYTECODE& _ps,
	const D3D12_BLEND_DESC& _blend_desc,
	const D3D12_RASTERIZER_DESC& _rasterizer_desc,
	const D3D12_DEPTH_STENCIL_DESC& _depth_stencil_desc,
	const D3D12_INPUT_LAYOUT_DESC& _input_layout_desc)
{
	const auto& root_signature_key = root_signatures_.find(_key);
	const auto& pipeline_state_key = pipeline_states_.find(_key);
	if ((root_signature_key != root_signatures_.end()) && (pipeline_state_key != pipeline_states_.end()))
	{
		Debug::Console(std::string(_key + " : already created.\n").c_str());
		return true;
	}

	// ルートシグネチャの作成
	D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {
		.NumParameters = _root_param_num,
		.pParameters = _root_params,
		.NumStaticSamplers = _static_sampler_num,
		.pStaticSamplers = _static_samplers,
		.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
	};
	ComPtr<ID3DBlob> blob, err;
	auto hr = D3D12SerializeRootSignature(
		&root_signature_desc, 
		D3D_ROOT_SIGNATURE_VERSION_1_0, 
		blob.GetAddressOf(), 
		err.GetAddressOf());
	if (FAILED(hr))
	{
		Debug::Console("D3D12SerializeRootSignature failed.\n");
		return false;
	}

	ComPtr<ID3D12RootSignature> root_signature;
	hr = _device->CreateRootSignature(
		0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&root_signature));
	if (FAILED(hr))
	{
		Debug::Console("CreateRootSignature failed.\n");
		return false;
	}
	root_signatures_.insert(std::make_pair(_key, root_signature));

	// パイプラインステートの作成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc = {
		.pRootSignature = root_signature.Get(),
		.VS = _vs,
		.PS = _ps,
		.DS = _ds,
		.HS = _hs,
		.GS = _gs,
		.BlendState = _blend_desc,
		.SampleMask = D3D12_DEFAULT_SAMPLE_MASK,
		.RasterizerState = _rasterizer_desc,
		.DepthStencilState = _depth_stencil_desc,
		.InputLayout = _input_layout_desc,
		.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
		.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		.NumRenderTargets = 1,
		.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM,
		.DSVFormat = DXGI_FORMAT_D32_FLOAT,
		.SampleDesc = { 1, 0 }
	};
	ComPtr<ID3D12PipelineState> pipeline_state;
	hr = _device->CreateGraphicsPipelineState(&pipeline_state_desc, IID_PPV_ARGS(&pipeline_state));
	if (FAILED(hr))
	{
		Debug::Console("CreateGraphicsPipelineState failed.\n");
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
