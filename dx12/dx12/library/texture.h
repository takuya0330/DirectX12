#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>

#include "../external/d3dx12/d3dx12.h"

#include "math.h"
#include "resource.h"

namespace snd
{
	class Texture
	{
	public:
		Texture() {}
		~Texture() {}

		Texture(ID3D12Device* _device);

		void Update(float2 _dst, float2 _dst_size, float2 _src, float2 _src_size, float _angle = 0, float4 _color = 1);

		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView()const { return vertex_buffer_view_; }
		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView()const { return index_buffer_view_; }

		static constexpr const D3D12_INPUT_ELEMENT_DESC kInputElement[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		static constexpr const snd::uint kInputElementNum = _countof(kInputElement);
	private:
		struct Vertex {
			float3 position_;
			float2 texcoord_;
			float4 color_;
		};
		detail::Resource vertex_buffer_;
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
		detail::Resource index_buffer_;
		D3D12_INDEX_BUFFER_VIEW index_buffer_view_;
		float2 tex_size_;
	};
}