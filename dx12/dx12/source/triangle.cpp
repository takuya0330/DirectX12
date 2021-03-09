#include "../include/triangle.h"
#include "../include/d3d12_helper.h"
#include "../include/utility.h"

Triangle::Triangle(ID3D12Device* _device)
{
	constexpr Vertex vertices[] = {
		{ float3(-0.5f,-0.5f, 0.0f), float2(0, 0), float4(0, 1, 0, 1) },
		{ float3( 0.0f, 0.5f, 0.0f), float2(0, 0), float4(1, 0, 0, 1) },
		{ float3( 0.5f,-0.5f, 0.0f), float2(0, 0), float4(0, 0, 1, 1) }
	};
	constexpr uint32_t indices[] = { 0,1,2 };

	// 頂点バッファーの作成
	auto heap_properties = cd3d12::HeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	auto vertex_buffer_desc = cd3d12::ResourceDesc(
		D3D12_RESOURCE_DIMENSION_BUFFER,
		sizeof(vertices), 
		1,
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE);
	auto hr = _device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&vertex_buffer_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateCommittedResource\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return;
	}
	if (!cd3d12::WriteUploadHeapMemory(vertex_buffer_.Get(), vertices, sizeof(vertices)))
	{
		debug::Console("Failed : WriteUploadHeapMemory\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return;
	}
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = sizeof(vertices);
	vertex_buffer_view_.StrideInBytes = sizeof(Vertex);

	// インデックスバッファーの作成
	auto index_buffer_desc = cd3d12::ResourceDesc(
		D3D12_RESOURCE_DIMENSION_BUFFER,
		sizeof(indices),
		1,
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE);
	hr = _device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&index_buffer_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&index_buffer_));
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateCommittedResource\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return;
	}
	if (!cd3d12::WriteUploadHeapMemory(index_buffer_.Get(), indices, sizeof(indices)))
	{
		debug::Console("Failed : WriteUploadHeapMemory\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return;
	}
	index_buffer_view_.BufferLocation = index_buffer_->GetGPUVirtualAddress();
	index_buffer_view_.Format = DXGI_FORMAT_R32_UINT;
	index_buffer_view_.SizeInBytes = sizeof(indices);
	index_count_ = _countof(indices);
}