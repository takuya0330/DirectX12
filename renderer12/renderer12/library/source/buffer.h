#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl/client.h>

#include "math.h"

namespace snd
{
	namespace detail
	{
		class Resource
		{
		public:
			Resource() {}
			virtual ~Resource() {}

			void Create(ID3D12Device* _device, uint _size);
			void Mapping(const void* _data, size_t _size);
		protected:
			Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
		};
	}

	class VertexBuffer : public detail::Resource
	{
	public:
		VertexBuffer() {}
		~VertexBuffer() {}

		void Setup(uint _size, uint _stride);

		D3D12_VERTEX_BUFFER_VIEW GetView()const { return view_; }
	private:
		D3D12_VERTEX_BUFFER_VIEW view_;
	};

	class IndexBuffer : public detail::Resource
	{
	public:
		IndexBuffer() {}
		~IndexBuffer() {}

		void Setup(uint _size);

		D3D12_INDEX_BUFFER_VIEW GetView()const { return view_; }
	private:
		D3D12_INDEX_BUFFER_VIEW view_;
	};
}