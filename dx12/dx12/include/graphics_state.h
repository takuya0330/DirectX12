#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>

enum class BlendMode
{
	eDefault,
	eAlpha,
	eAddition,
	eSubtract,
	eReplace,
	eMultiply,
	eLighten,
	eDarken,
	eScreen,
	eMax
};
inline constexpr D3D12_BLEND_DESC BlendState(BlendMode _BlendMode)
{
	D3D12_RENDER_TARGET_BLEND_DESC render_target_blend_desc{};
	switch (_BlendMode)
	{
	case BlendMode::eDefault:
		render_target_blend_desc.BlendEnable = FALSE;
		render_target_blend_desc.LogicOpEnable = FALSE;
		render_target_blend_desc.SrcBlend = D3D12_BLEND_ONE;
		render_target_blend_desc.DestBlend = D3D12_BLEND_ZERO;
		render_target_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
		render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
		render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;
	case BlendMode::eAlpha:
		render_target_blend_desc.BlendEnable = TRUE;
		render_target_blend_desc.LogicOpEnable = FALSE;
		render_target_blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		render_target_blend_desc.DestBlend = D3D12_BLEND_INV_DEST_ALPHA;
		render_target_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
		render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
		render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;
	case BlendMode::eAddition:
		render_target_blend_desc.BlendEnable = TRUE;
		render_target_blend_desc.LogicOpEnable = FALSE;
		render_target_blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		render_target_blend_desc.DestBlend = D3D12_BLEND_ONE;
		render_target_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ZERO;
		render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
		render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;
	case BlendMode::eSubtract:
		render_target_blend_desc.BlendEnable = TRUE;
		render_target_blend_desc.LogicOpEnable = FALSE;
		render_target_blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		render_target_blend_desc.DestBlend = D3D12_BLEND_ONE;
		render_target_blend_desc.BlendOp = D3D12_BLEND_OP_SUBTRACT;
		render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ZERO;
		render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
		render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;
	case BlendMode::eReplace:
		render_target_blend_desc.BlendEnable = TRUE;
		render_target_blend_desc.LogicOpEnable = FALSE;
		render_target_blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		render_target_blend_desc.DestBlend = D3D12_BLEND_ZERO;
		render_target_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
		render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
		render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;
	case BlendMode::eMultiply:
		render_target_blend_desc.BlendEnable = TRUE;
		render_target_blend_desc.LogicOpEnable = FALSE;
		render_target_blend_desc.SrcBlend = D3D12_BLEND_ZERO;
		render_target_blend_desc.DestBlend = D3D12_BLEND_SRC_COLOR;
		render_target_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_DEST_ALPHA;
		render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
		render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
		render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;
	case BlendMode::eLighten:
		render_target_blend_desc.BlendEnable = TRUE;
		render_target_blend_desc.LogicOpEnable = FALSE;
		render_target_blend_desc.SrcBlend = D3D12_BLEND_ONE;
		render_target_blend_desc.DestBlend = D3D12_BLEND_ONE;
		render_target_blend_desc.BlendOp = D3D12_BLEND_OP_MAX;
		render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_MAX;
		render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
		render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;
	case BlendMode::eDarken:
		render_target_blend_desc.BlendEnable = TRUE;
		render_target_blend_desc.LogicOpEnable = FALSE;
		render_target_blend_desc.SrcBlend = D3D12_BLEND_ONE;
		render_target_blend_desc.DestBlend = D3D12_BLEND_ONE;
		render_target_blend_desc.BlendOp = D3D12_BLEND_OP_MIN;
		render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_MIN;
		render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
		render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;
	case BlendMode::eScreen:
		render_target_blend_desc.BlendEnable = TRUE;
		render_target_blend_desc.LogicOpEnable = FALSE;
		render_target_blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		render_target_blend_desc.DestBlend = D3D12_BLEND_INV_SRC_COLOR;
		render_target_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
		render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
		render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;
	}
	D3D12_BLEND_DESC blend_desc{};
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < 8; ++i)
	{
		blend_desc.RenderTarget[i] = render_target_blend_desc;
	}
	return blend_desc;
}

enum class DepthStencilMode
{
	e2D,
	e3D,
	eMax
};
inline constexpr D3D12_DEPTH_STENCIL_DESC DepthStencilState(DepthStencilMode _DepthStencilMode)
{
	D3D12_DEPTH_STENCIL_DESC depth_stencil_desc{};
	switch (_DepthStencilMode)
	{
	case DepthStencilMode::e2D:
		depth_stencil_desc.DepthEnable = FALSE;
		depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		depth_stencil_desc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		depth_stencil_desc.StencilEnable = FALSE;
		depth_stencil_desc.StencilReadMask = 0xFF;
		depth_stencil_desc.StencilWriteMask = 0xFF;
		depth_stencil_desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
		depth_stencil_desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		depth_stencil_desc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
		depth_stencil_desc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		break;
	case DepthStencilMode::e3D:
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		depth_stencil_desc.StencilEnable = FALSE;
		depth_stencil_desc.StencilReadMask = 0xFF;
		depth_stencil_desc.StencilWriteMask = 0xFF;
		depth_stencil_desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
		depth_stencil_desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		depth_stencil_desc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
		depth_stencil_desc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		break;
	}
	return depth_stencil_desc;
}

enum class RasterizerMode
{
	e2D,
	e3DFront,
	e3DBack,
	eMax
};
inline constexpr D3D12_RASTERIZER_DESC RasterizerState(RasterizerMode _RasterizerMode, D3D12_FILL_MODE _FillMode = D3D12_FILL_MODE_SOLID)
{
	D3D12_RASTERIZER_DESC rasterizer_desc{};
	switch (_RasterizerMode)
	{
	case RasterizerMode::e2D:
		rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterizer_desc.CullMode = D3D12_CULL_MODE_NONE;
		rasterizer_desc.FrontCounterClockwise = FALSE;
		rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		rasterizer_desc.ForcedSampleCount = 0;
		rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		break;
	case RasterizerMode::e3DFront:
		rasterizer_desc.FillMode = _FillMode;
		rasterizer_desc.CullMode = D3D12_CULL_MODE_FRONT;
		rasterizer_desc.FrontCounterClockwise = FALSE;
		rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		rasterizer_desc.ForcedSampleCount = 0;
		rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		break;
	case RasterizerMode::e3DBack:
		rasterizer_desc.FillMode = _FillMode;
		rasterizer_desc.CullMode = D3D12_CULL_MODE_BACK;
		rasterizer_desc.FrontCounterClockwise = TRUE;
		rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		rasterizer_desc.ForcedSampleCount = 0;
		rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		break;
	}
	return rasterizer_desc;
}

enum class SamplerMode
{
	eAnisotropic,
	eLiner,
	ePoint,
	eClamp,
	eBorder,
	eMax
};
inline constexpr D3D12_SAMPLER_DESC SamplerState(SamplerMode _SamplerMode)
{
	D3D12_SAMPLER_DESC sampler_desc{};
	switch (_SamplerMode)
	{
	case SamplerMode::eAnisotropic:
		sampler_desc.Filter = D3D12_FILTER_ANISOTROPIC;
		sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 16;
		sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		sampler_desc.BorderColor[0] = 0.0f;
		sampler_desc.BorderColor[1] = 0.0f;
		sampler_desc.BorderColor[2] = 0.0f;
		sampler_desc.BorderColor[3] = 0.0f;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		break;
	case SamplerMode::eLiner:
		sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 16;
		sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		sampler_desc.BorderColor[0] = 0.0f;
		sampler_desc.BorderColor[1] = 0.0f;
		sampler_desc.BorderColor[2] = 0.0f;
		sampler_desc.BorderColor[3] = 0.0f;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		break;
	case SamplerMode::ePoint:
		sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 16;
		sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		sampler_desc.BorderColor[0] = 0.0f;
		sampler_desc.BorderColor[1] = 0.0f;
		sampler_desc.BorderColor[2] = 0.0f;
		sampler_desc.BorderColor[3] = 0.0f;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		break;
	case SamplerMode::eClamp:
		sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 16;
		sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		sampler_desc.BorderColor[0] = 0.0f;
		sampler_desc.BorderColor[1] = 0.0f;
		sampler_desc.BorderColor[2] = 0.0f;
		sampler_desc.BorderColor[3] = 0.0f;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		break;
	case SamplerMode::eBorder:	// ToDo.
		//sampler_desc.Filter = D3D12_FILTER_ANISOTROPIC;
		//sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//sampler_desc.MipLODBias = 0.0f;
		//sampler_desc.MaxAnisotropy = 16;
		//sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		//sampler_desc.BorderColor[0] = 1.0f;
		//sampler_desc.BorderColor[1] = 1.0f;
		//sampler_desc.BorderColor[2] = 1.0f;
		//sampler_desc.BorderColor[3] = 1.0f;
		//sampler_desc.MinLOD = 0;
		//sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		break;
	}
	return sampler_desc;
}
inline constexpr D3D12_STATIC_SAMPLER_DESC StaticSamplerState(SamplerMode _SamplerMode, UINT _ShaderRegister = 0)
{
	D3D12_STATIC_SAMPLER_DESC static_sampler_desc{};
	switch (_SamplerMode)
	{
	case SamplerMode::eAnisotropic:
		static_sampler_desc.Filter = D3D12_FILTER_ANISOTROPIC;
		static_sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		static_sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		static_sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		static_sampler_desc.MipLODBias = 0.0f;
		static_sampler_desc.MaxAnisotropy = 16;
		static_sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		static_sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		static_sampler_desc.MinLOD = 0;
		static_sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		static_sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		static_sampler_desc.ShaderRegister = _ShaderRegister;
		static_sampler_desc.RegisterSpace = 0;
		static_sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		break;
	case SamplerMode::eLiner:
		static_sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		static_sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		static_sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		static_sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		static_sampler_desc.MipLODBias = 0.0f;
		static_sampler_desc.MaxAnisotropy = 16;
		static_sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		static_sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		static_sampler_desc.MinLOD = 0;
		static_sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		static_sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		static_sampler_desc.ShaderRegister = _ShaderRegister;
		static_sampler_desc.RegisterSpace = 0;
		static_sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		break;
	case SamplerMode::ePoint:
		static_sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		static_sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		static_sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		static_sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		static_sampler_desc.MipLODBias = 0.0f;
		static_sampler_desc.MaxAnisotropy = 16;
		static_sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		static_sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		static_sampler_desc.MinLOD = 0;
		static_sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		static_sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		static_sampler_desc.ShaderRegister = _ShaderRegister;
		static_sampler_desc.RegisterSpace = 0;
		static_sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		break;
	case SamplerMode::eClamp:
		static_sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		static_sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		static_sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		static_sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		static_sampler_desc.MipLODBias = 0.0f;
		static_sampler_desc.MaxAnisotropy = 16;
		static_sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		static_sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		static_sampler_desc.MinLOD = 0;
		static_sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		static_sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		static_sampler_desc.ShaderRegister = _ShaderRegister;
		static_sampler_desc.RegisterSpace = 0;
		static_sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		break;
	case SamplerMode::eBorder:	// ToDo.
		//static_sampler_desc.Filter = D3D12_FILTER_ANISOTROPIC;
		//static_sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//static_sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//static_sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//static_sampler_desc.MipLODBias = 0.0f;
		//static_sampler_desc.MaxAnisotropy = 16;
		//static_sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		//static_sampler_desc.BorderColor[0] = 1.0f;
		//static_sampler_desc.BorderColor[1] = 1.0f;
		//static_sampler_desc.BorderColor[2] = 1.0f;
		//static_sampler_desc.BorderColor[3] = 1.0f;
		//static_sampler_desc.MinLOD = 0;
		//static_sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		break;
	}
	return static_sampler_desc;
}
