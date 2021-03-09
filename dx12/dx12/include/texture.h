#pragma once

#include "geometry2d.h"
#include "math.h"

class D3D12Manager;

class Texture : public Geometry2D
{
public:
	Texture(D3D12Manager* _d3d12, const char* _filename);

	void Draw(ID3D12GraphicsCommandList* _command_list)override
	{
		_command_list->SetDescriptorHeaps(1, texture_heap_.GetAddressOf());
		_command_list->SetGraphicsRootDescriptorTable(0, texture_handle_);
		_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_command_list->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
		_command_list->IASetIndexBuffer(&index_buffer_view_);
		_command_list->DrawIndexedInstanced(index_count_, 1, 0, 0, 0);
	}

private:
	int2 size_;
	ComPtr<ID3D12Resource> texture_buffer_;
	ComPtr<ID3D12DescriptorHeap> texture_heap_;
	D3D12_GPU_DESCRIPTOR_HANDLE texture_handle_;
};