#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>

#include "math.h"

namespace snd
{
	class PipelineState
	{
	public:
		PipelineState() {}
		~PipelineState() {}

		ID3D12PipelineState* Get()const { return pipeline_state_.Get(); }
		ID3D12PipelineState* operator->()const { return pipeline_state_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipeline_state_;
	};
}