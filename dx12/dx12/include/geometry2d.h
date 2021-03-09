#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "math.h"

class Geometry2D
{
public:
	Geometry2D() = default;
	virtual ~Geometry2D() {};

	virtual void Draw(ID3D12GraphicsCommandList* _command_list)
	{
		_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_command_list->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
		_command_list->IASetIndexBuffer(&index_buffer_view_);
		_command_list->DrawIndexedInstanced(index_count_, 1, 0, 0, 0);
	}

	static constexpr D3D12_INPUT_ELEMENT_DESC kInputElement[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	static constexpr UINT kInputElementNum = _countof(kInputElement);
protected:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct Vertex
	{
		float3 position_ = 0;
		float2 texcoord_ = 0;
		float4 color_ = 0;
	};

	ComPtr<ID3D12Resource> vertex_buffer_;
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_ = {};

	ComPtr<ID3D12Resource> index_buffer_;
	D3D12_INDEX_BUFFER_VIEW index_buffer_view_ = {};
	UINT index_count_ = 0;
};