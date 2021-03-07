#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "math.h"
#include "property.h"

struct Vertex2D
{
	float3 position;
	float4 color;
};

class Triangle
{
public:
	Triangle() = default;
	~Triangle() = default;

	bool Initialize(ID3D12Device* _device);
	void Draw(ID3D12GraphicsCommandList* _command_list);

	static constexpr D3D12_INPUT_ELEMENT_DESC kInputElement[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	static constexpr UINT kInputElementNum = _countof(kInputElement);
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> vertex_buffer_;
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_ = {};

	ComPtr<ID3D12Resource> index_buffer_;
	D3D12_INDEX_BUFFER_VIEW index_buffer_view_ = {};
};