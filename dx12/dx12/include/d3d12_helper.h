#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include "utility.h"

namespace cd3d12
{
	inline HRESULT CreateDescriptorHeap(
		ID3D12Device* _device,
		D3D12_DESCRIPTOR_HEAP_TYPE _heap_type,
		UINT _num,
		D3D12_DESCRIPTOR_HEAP_FLAGS _heap_flags,
		ID3D12DescriptorHeap** _heap)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {
			.Type = _heap_type,
			.NumDescriptors = _num,
			.Flags = _heap_flags,
			.NodeMask = 0
		};
		return _device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(_heap));
	}

	inline D3D12_HEAP_PROPERTIES HeapProperties(D3D12_HEAP_TYPE _type)
	{
		D3D12_HEAP_PROPERTIES properties = {
			.Type = _type,
			.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,// ヒープのCPUページプロパティは不明
			.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,	// ヒープのメモリプールは不明
			.CreationNodeMask = 1,										// 0,1どちらでも同じ
			.VisibleNodeMask = 1											// CreationNodeMaskと同じ値である必要がある
		};
		return properties;
	}

	inline D3D12_RESOURCE_DESC ResourceDesc(
		D3D12_RESOURCE_DIMENSION _dimension, 
		UINT64 _width,
		UINT _height,
		UINT16 _depth_or_array_size,
		UINT16 _miplevels,
		DXGI_FORMAT _format,
		D3D12_TEXTURE_LAYOUT _layout,
		D3D12_RESOURCE_FLAGS _flags)
	{
		D3D12_RESOURCE_DESC desc = {
			.Dimension = _dimension,
			.Alignment = 0,
			.Width = _width,
			.Height = _height,
			.DepthOrArraySize = _depth_or_array_size,
			.MipLevels = _miplevels,
			.Format = _format,
			.SampleDesc = {.Count = 1, .Quality = 0 },
			.Layout = _layout,
			.Flags = _flags
		};
		return desc;
	}

	inline bool WriteUploadHeapMemory(ID3D12Resource* _resource, const void* _data, UINT32 _size)
	{
		void* mapped = nullptr;
		auto hr = _resource->Map(0, nullptr, &mapped);
		if (FAILED(hr)) return false;

		memcpy(mapped, _data, _size);
		_resource->Unmap(0, nullptr);

		return true;
	}

	inline D3D12_RESOURCE_BARRIER BarrierTransition(
		ID3D12Resource* _resource,
		D3D12_RESOURCE_STATES _before,
		D3D12_RESOURCE_STATES _after)
	{
		D3D12_RESOURCE_BARRIER barrier =
		{
			.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
			.Transition =
			{
				.pResource = _resource,
				.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
				.StateBefore = _before,
				.StateAfter = _after
			}
		};
		return barrier;
	}

	inline D3D12_DESCRIPTOR_RANGE DescriptorRange(
		D3D12_DESCRIPTOR_RANGE_TYPE _type,
		UINT _descriptor_num,
		UINT _base_shader_register,
		UINT _register_space = 0)
	{
		D3D12_DESCRIPTOR_RANGE descriptor_range = {
			.RangeType = _type,
			.NumDescriptors = _descriptor_num,
			.BaseShaderRegister = _base_shader_register,
			.RegisterSpace = _register_space,
			.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
		};
		return descriptor_range;
	}

	inline D3D12_ROOT_DESCRIPTOR_TABLE RootDescriptorTable(
		UINT _range_num,
		const D3D12_DESCRIPTOR_RANGE* _range)
	{
		D3D12_ROOT_DESCRIPTOR_TABLE descriptor_table = {
			.NumDescriptorRanges = _range_num,
			.pDescriptorRanges = _range
		};
		return descriptor_table;
	}

	inline D3D12_ROOT_PARAMETER RootParamterDescriptorTable(
		const D3D12_ROOT_DESCRIPTOR_TABLE& _table,
		D3D12_SHADER_VISIBILITY _shader_visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		D3D12_ROOT_PARAMETER root_parameter = {
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
			.DescriptorTable = _table,
			.ShaderVisibility = _shader_visibility
		};
		return root_parameter;
	}

	inline D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		UINT _parameter_num, 
		const D3D12_ROOT_PARAMETER* _parameters, 
		UINT _static_sampler_num, 
		const D3D12_STATIC_SAMPLER_DESC* _static_samplers)
	{
		D3D12_ROOT_SIGNATURE_DESC desc = {
			.NumParameters = _parameter_num,
			.pParameters = _parameters,
			.NumStaticSamplers = _static_sampler_num,
			.pStaticSamplers = _static_samplers,
			.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
		};
		return desc;
	}

	inline D3D12_SHADER_BYTECODE ShaderByteCode(ID3DBlob* _blob)
	{
		return { _blob->GetBufferPointer(), _blob->GetBufferSize() };
	}

	inline D3D12_INPUT_LAYOUT_DESC InputLayoutDesc(const D3D12_INPUT_ELEMENT_DESC* _desc, UINT _num)
	{
		return { _desc, _num };
	}

	inline D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc(
		ID3D12RootSignature* _root_signature,
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
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {
			.pRootSignature = _root_signature,
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
		return desc;
	}

	inline HRESULT D3DCompile(const wchar_t* _filename, const char* _shader_model, ID3DBlob** _blob)
	{
		UINT compile_flag = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		compile_flag |= D3DCOMPILE_DEBUG;
#endif
		Microsoft::WRL::ComPtr<ID3DBlob> err;
		auto hr = D3DCompileFromFile(
			_filename,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			_shader_model,
			compile_flag,
			0,
			_blob,
			err.ReleaseAndGetAddressOf());
		if (err)
		{
			debug::Console(static_cast<char*>(err->GetBufferPointer()));
			return S_FALSE;
		}
		return hr;
	}
}