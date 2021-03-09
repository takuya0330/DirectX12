#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb/stb_image.h"

#include "../include/d3d12_manager.h"
#include "../include/texture.h"
#include "../include/d3d12_helper.h"
#include "../include/utility.h"

Texture::Texture(D3D12Manager* _d3d12, const char* _filename)
{
	ID3D12Device* device = _d3d12->Device;
	ID3D12CommandQueue* command_queue = _d3d12->CommandQueue;
	ID3D12GraphicsCommandList* command_list = _d3d12->GraphicsCommandList;

	constexpr Vertex vertices[] = {
		{ float3(-0.5f,  0.5f, 0), float2(0, 0), float4(1, 1, 1, 1) },
		{ float3( 0.5f,  0.5f, 0), float2(1, 0), float4(1, 1, 1, 1) },
		{ float3(-0.5f, -0.5f, 0), float2(0, 1), float4(1, 1, 1, 1) },
		{ float3( 0.5f, -0.5f, 0), float2(1, 1), float4(1, 1, 1, 1) }
	};
	constexpr uint32_t indices[] = { 0,1,2,2,1,3 };

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
	auto hr = device->CreateCommittedResource(
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
	hr = device->CreateCommittedResource(
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

	// テクスチャのロード
	int channels = 0;
	auto* image = stbi_load(_filename, &size_.x, &size_.y, &channels, 0);

	// テクスチャバッファの作成
	auto texture_properties = cd3d12::HeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	auto texture_buffer_desc = cd3d12::ResourceDesc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		size_.x,
		size_.y,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_NONE);
	hr = device->CreateCommittedResource(
		&texture_properties,
		D3D12_HEAP_FLAG_NONE,
		&texture_buffer_desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&texture_buffer_));
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateCommittedResource\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return;
	}

	// ステージングバッファの作成
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts = {};
	UINT row_num = 0;
	UINT64 row_size_in_bytes = 0, total_bytes = 0;
	device->GetCopyableFootprints(&texture_buffer_desc, 0, 1, 0, &layouts, &row_num, &row_size_in_bytes, &total_bytes);

	ComPtr<ID3D12Resource> staging_buffer;
	auto staging_properties = cd3d12::HeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	auto staging_buffer_desc = cd3d12::ResourceDesc(
		D3D12_RESOURCE_DIMENSION_BUFFER,
		total_bytes,
		1,
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE);
	hr = device->CreateCommittedResource(
		&staging_properties,
		D3D12_HEAP_FLAG_NONE,
		&staging_buffer_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&staging_buffer));
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateCommittedResource\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return;
	}

	// ステージングバッファに画像データをコピー
	const uint image_pitch = size_.x * sizeof(uint);
	void* mapped = nullptr;
	hr = staging_buffer->Map(0, nullptr, &mapped);
	if (FAILED(hr))
	{
		debug::Console("Failed : Map\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return;
	}
	for (UINT h = 0; h < row_num; ++h)
	{
		auto dst = static_cast<char*>(mapped) + h * row_size_in_bytes;
		auto src = image + h * image_pitch;
		memcpy(dst, src, image_pitch);
	}
	staging_buffer->Unmap(0, nullptr);

	// コマンドリセット
	_d3d12->Reset();

	ComPtr<ID3D12Fence1> fence;
	hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateFence\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return;
	}
	// データのコピー
	D3D12_TEXTURE_COPY_LOCATION upload_location = {
		.pResource = texture_buffer_.Get(),
		.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
		.SubresourceIndex = 0
	};
	D3D12_TEXTURE_COPY_LOCATION copy_location = {
		.pResource = staging_buffer.Get(),
		.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
		.PlacedFootprint = layouts,
	};
	command_list->CopyTextureRegion(&upload_location, 0, 0, 0, &copy_location, nullptr);

	auto barrier = cd3d12::BarrierTransition(
		texture_buffer_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	command_list->ResourceBarrier(1, &barrier);

	_d3d12->ExecuteCommandQueue();

	const UINT64 expected = 1;
	command_queue->Signal(fence.Get(), expected);
	while (fence->GetCompletedValue() != expected)
	{
		Sleep(1);
	}

	stbi_image_free(image);

	// シェーダーリソースビューの作成
	hr = cd3d12::CreateDescriptorHeap(
		device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		&texture_heap_);
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateDescriptorHeap\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return;
	}
	D3D12_CPU_DESCRIPTOR_HANDLE texture_handle = texture_heap_->GetCPUDescriptorHandleForHeapStart();
	texture_handle_ = texture_heap_->GetGPUDescriptorHandleForHeapStart();
	constexpr D3D12_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D,
		.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
		.Texture2D = {.MipLevels = 1 }
	};
	device->CreateShaderResourceView(
		texture_buffer_.Get(),
		&shader_resource_view_desc,
		texture_handle);
}