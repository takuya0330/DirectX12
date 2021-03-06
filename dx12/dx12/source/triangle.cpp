#include "../include/triangle.h"
#include "../include/utility.h"

bool Triangle::Initialize(ID3D12Device* _device)
{
	constexpr Vertex2D vertices[] = {
		{ float3(-0.5f,-0.5f, 0.0f), float4(0, 1, 0, 1) },
		{ float3( 0.0f, 0.5f, 0.0f), float4(1, 0, 0, 1) },
		{ float3( 0.5f,-0.5f, 0.0f), float4(0, 0, 1, 1) }
	};
	constexpr uint32_t indices[] = { 0,1,2 };

	// 頂点バッファーの作成
	constexpr D3D12_HEAP_PROPERTIES heap_properties = {
		.Type = D3D12_HEAP_TYPE_UPLOAD,							
		.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,	
		.CreationNodeMask = 1,										
		.VisibleNodeMask = 1											
	};
	D3D12_RESOURCE_DESC vertex_buffer_desc = {
		.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,	
		.Alignment = 0,												
		.Width = sizeof(vertices),
		.Height = 1,											
		.DepthOrArraySize = 1,										
		.MipLevels = 1,												
		.Format = DXGI_FORMAT_UNKNOWN,							
		.SampleDesc =
		{
			.Count = 1,													
			.Quality = 0												
		},
		.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,				
		.Flags = D3D12_RESOURCE_FLAG_NONE	
	};
	auto hr = _device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&vertex_buffer_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	if (FAILED(hr))
	{
		Debug::Console("CreateCommittedResource failed.");
		return false;
	}
	if (!WriteUploadHeapMemory(vertex_buffer_.Get(), vertices, sizeof(vertices))) return false;
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = sizeof(vertices);
	vertex_buffer_view_.StrideInBytes = sizeof(Vertex2D);

	// インデックスバッファーの作成
	D3D12_RESOURCE_DESC index_buffer_desc = {
		.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
		.Alignment = 0,
		.Width = sizeof(indices),
		.Height = 1,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = DXGI_FORMAT_UNKNOWN,
		.SampleDesc =
		{
			.Count = 1,
			.Quality = 0
		},
		.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		.Flags = D3D12_RESOURCE_FLAG_NONE
	};
	hr = _device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&index_buffer_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&index_buffer_));
	if (FAILED(hr))
	{
		Debug::Console("CreateCommittedResource failed.");
		return false;
	}
	if (!WriteUploadHeapMemory(index_buffer_.Get(), indices, sizeof(indices))) return false;
	index_buffer_view_.BufferLocation = index_buffer_->GetGPUVirtualAddress();
	index_buffer_view_.Format = DXGI_FORMAT_R32_UINT;
	index_buffer_view_.SizeInBytes = sizeof(indices);

	return true;
}

void Triangle::Draw(ID3D12GraphicsCommandList* _command_list)
{
	_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_command_list->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
	_command_list->IASetIndexBuffer(&index_buffer_view_);

	_command_list->DrawIndexedInstanced(3, 1, 0, 0, 0);
}