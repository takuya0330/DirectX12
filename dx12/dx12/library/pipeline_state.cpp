#include "pipeline_state.h"
#include "root_signature.h"
#include "utility.h"

namespace snd
{
	void PipelineState::SetBlendState(const D3D12_BLEND_DESC& _desc)
	{
		pipeline_state_desc_.BlendState = _desc;
	}

	void PipelineState::SetRasterizerState(const D3D12_RASTERIZER_DESC& _desc)
	{
		pipeline_state_desc_.RasterizerState = _desc;
	}

	void PipelineState::SetRenderTargets(uint _num, const DXGI_FORMAT _format)
	{
		pipeline_state_desc_.NumRenderTargets = _num;
		pipeline_state_desc_.RTVFormats[0] = _format;
	}

	void PipelineState::SetDepthStencil(const DXGI_FORMAT _format, const D3D12_DEPTH_STENCIL_DESC& _desc)
	{
		pipeline_state_desc_.DSVFormat = _format;
		pipeline_state_desc_.DepthStencilState = _desc;
	}

	void PipelineState::SetInputLayout(const D3D12_INPUT_ELEMENT_DESC* _desc, uint _count)
	{
		pipeline_state_desc_.InputLayout = { _desc, _count };
	}

	void PipelineState::SetPrimitiveTopology(const D3D12_PRIMITIVE_TOPOLOGY_TYPE _type)
	{
		pipeline_state_desc_.PrimitiveTopologyType = _type;
	}

	void PipelineState::SetVertexShader(ID3DBlob* _blob)
	{
		pipeline_state_desc_.VS = CD3DX12_SHADER_BYTECODE(_blob);
	}

	void PipelineState::SetPixelShader(ID3DBlob* _blob)
	{
		pipeline_state_desc_.PS = CD3DX12_SHADER_BYTECODE(_blob);
	}

	void PipelineState::SetGeometryShader(ID3DBlob* _blob)
	{
		pipeline_state_desc_.GS = CD3DX12_SHADER_BYTECODE(_blob);
	}

	void PipelineState::SetHullShader(ID3DBlob* _blob)
	{
		pipeline_state_desc_.HS = CD3DX12_SHADER_BYTECODE(_blob);
	}

	void PipelineState::SetDomainShader(ID3DBlob* _blob)
	{
		pipeline_state_desc_.DS = CD3DX12_SHADER_BYTECODE(_blob);
	}

	void PipelineState::Setup(ID3D12Device* _device, const RootSignature& _root_signature)
	{
		pipeline_state_desc_.pRootSignature = _root_signature.Get();

		// マルチサンプル設定
		pipeline_state_desc_.SampleDesc = { 1,0 };
		pipeline_state_desc_.SampleMask = UINT_MAX; // UINT_MAX, D3D12_DEFAULT_SAMPLE_MASK

		ASSERT_SUCCEEDED(_device->CreateGraphicsPipelineState(&pipeline_state_desc_, IID_PPV_ARGS(&pipeline_state_)));
	}
}