#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>

#include "../external/d3dx12/d3dx12.h"

#include "math.h"

namespace snd
{
	class RootSignature;

	class PipelineState
	{
	public:
		PipelineState() {}
		~PipelineState() {}

		void SetBlendState(const D3D12_BLEND_DESC& _desc);
		void SetRasterizerState(const D3D12_RASTERIZER_DESC& _desc);
		void SetRenderTargets(uint _num, const DXGI_FORMAT _format);
		void SetDepthStencil(const DXGI_FORMAT _format, const D3D12_DEPTH_STENCIL_DESC& _desc);
		void SetInputLayout(const D3D12_INPUT_ELEMENT_DESC* _desc, uint _count);
		void SetPrimitiveTopology(const D3D12_PRIMITIVE_TOPOLOGY_TYPE _type);

		void SetVertexShader(ID3DBlob* _blob);
		void SetPixelShader(ID3DBlob* _blob);
		void SetGeometryShader(ID3DBlob* _blob);
		void SetHullShader(ID3DBlob* _blob);
		void SetDomainShader(ID3DBlob* _blob);
		
		void Setup(ID3D12Device* _device, const RootSignature& _root_signature);

		ID3D12PipelineState* Get()const { return pipeline_state_.Get(); }
		ID3D12PipelineState* operator->()const { return pipeline_state_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipeline_state_;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc_ = {};
	};
}