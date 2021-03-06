#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl.h>
#include <unordered_map>

using namespace Microsoft::WRL;

class PipelineManager
{
public:
	PipelineManager() = default;
	~PipelineManager() = default;

	bool Initialize(
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
		const D3D12_INPUT_LAYOUT_DESC& _input_layout_desc);
	void Bind(ID3D12GraphicsCommandList* _command_list, const std::string& _key);
private:
	std::unordered_map<std::string, ComPtr<ID3D12RootSignature>> root_signatures_;
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> pipeline_states_;
};