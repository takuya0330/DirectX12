#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>

#include "math.h"

namespace snd
{
	class VertexBuffer
	{
		friend class GraphicsCore;
	public:
		VertexBuffer() {}
		~VertexBuffer() {}

		D3D12_VERTEX_BUFFER_VIEW GetView()const { return view_; }
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
		D3D12_VERTEX_BUFFER_VIEW view_;
	};

	class IndexBuffer
	{
		friend class GraphicsCore;
	public:
		IndexBuffer() {}
		~IndexBuffer() {}

		D3D12_INDEX_BUFFER_VIEW GetView()const { return view_; }
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
		D3D12_INDEX_BUFFER_VIEW view_;
	};
}