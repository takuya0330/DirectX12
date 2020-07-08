//#include "texture.h"
//#include "utility.h"
//
//Texture::Texture(ID3D12Device* device)
//{
//	Vertex vertices[] = {
//		{ float3(-1, +1, 0), float2(0, 0) },
//		{ float3(+1, +1, 0), float2(1, 0) },
//		{ float3(-1, -1, 0), float2(0, 1) },
//		{ float3(+1, -1, 0), float2(1, 1) },
//	};
//	HRESULT hr = device->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertex_buffer_));
//
//	void* mapped;
//	CD3DX12_RANGE range(0, 0);
//	hr = vertex_buffer_->Map(0, &range, &mapped);
//	if (SUCCEEDED(hr))
//	{
//		memcpy(mapped, vertices, sizeof(vertices));
//		vertex_buffer_->Unmap(0, nullptr);
//	}
//
//	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
//	vertex_buffer_view_.SizeInBytes = sizeof(vertices);
//	vertex_buffer_view_.StrideInBytes = sizeof(Vertex);
//}