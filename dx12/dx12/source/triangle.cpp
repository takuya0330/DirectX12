#include "../include/triangle.h"
#include "../include/d3d12_helper.h"
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
	auto hr = cd3d12::CreateResource(
		_device,
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_DIMENSION_BUFFER,
		sizeof(vertices),
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		&vertex_buffer_);
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateResource\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return false;
	}
	if (!cd3d12::WriteUploadHeapMemory(vertex_buffer_.Get(), vertices, sizeof(vertices)))
	{
		debug::Console("Failed : WriteUploadHeapMemory\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return false;
	}
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = sizeof(vertices);
	vertex_buffer_view_.StrideInBytes = sizeof(Vertex2D);

	// インデックスバッファーの作成
	hr = cd3d12::CreateResource(
		_device,
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_DIMENSION_BUFFER,
		sizeof(indices),
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		&index_buffer_);
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateResource\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return false;
	}
	if (!cd3d12::WriteUploadHeapMemory(index_buffer_.Get(), indices, sizeof(indices)))
	{
		debug::Console("Failed : WriteUploadHeapMemory\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return false;
	}
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