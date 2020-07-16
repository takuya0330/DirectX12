#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>

#include "math.h"

namespace snd
{
	class PipelineStateObject
	{
		friend class GraphicsCore;
	public:
		PipelineStateObject() {}
		~PipelineStateObject() {}

		bool CompileShader(const wchar_t* _filename, const char* _shadermodel, ID3DBlob** _blob);
		bool CompileShader(const wchar_t* _filename, const char* _entrypoint, const char* _shadermodel, ID3DBlob** _blob);

		ID3D12PipelineState* GetPipelineState()const { return pipeline_state_.Get(); }
		ID3D12RootSignature* GetRootSignature()const { return root_signature_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipeline_state_;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> root_signature_;
	};
}