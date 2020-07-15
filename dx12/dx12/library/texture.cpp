#include "texture.h"
#include "utility.h"
#include "constants.h"

namespace snd
{
	Texture::Texture(ID3D12Device* _device)
	{
		Vertex vertices[] =
		{
			{ float3(-1,+1,0), float2(0,0), float4(1,1,1,1) },
			{ float3(+1,+1,0), float2(1,0), float4(1,1,1,1) },
			{ float3(-1,-1,0), float2(0,1), float4(1,1,1,1) },
			{ float3(+1,-1,0), float2(1,1), float4(1,1,1,1) }
		};
		uint indices[] = { 0,1,2,2,1,3 };

		vertex_buffer_.Create(_device, sizeof(vertices), vertices);
		vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();	// 頂点バッファーのアドレス
		vertex_buffer_view_.SizeInBytes = sizeof(vertices);									// 総バイト数
		vertex_buffer_view_.StrideInBytes = sizeof(vertices[0]);								// 1頂点当たりのバイト

		index_buffer_.Create(_device, sizeof(indices), indices);
		index_buffer_view_.BufferLocation = index_buffer_->GetGPUVirtualAddress();	// 頂点バッファーのアドレス
		index_buffer_view_.SizeInBytes = sizeof(indices);									// 総バイト数
		index_buffer_view_.Format = DXGI_FORMAT_R32_UINT;

		tex_size_ = 1;
	}

	void Texture::Update(float2 _dst, float2 _dst_size, float2 _src, float2 _src_size, float _angle, float4 _color)
	{
		float2 center(_dst + _dst_size * 0.5f);

		auto RotateX = [&](float _x, float _y, float _angle)
		{
			float x = _x - center.x;
			float y = _y - center.y;
			x = x * cosf(_angle) - y * sinf(_angle);
			return	x + center.x;
		};
		auto RotateY = [&](float _x, float _y, float _angle)
		{
			float x = _x - center.x;
			float y = _y - center.y;
			y = y * cosf(_angle) + x * sinf(_angle);
			return	y + center.y;
		};

		void* data;
		ASSERT_SUCCEEDED(vertex_buffer_->Map(0, nullptr, &data));

		Vertex* vertices = static_cast<Vertex*>(data);
		{
			vertices[0].position_.x = RotateX(_dst.x, _dst.y, _angle);
			vertices[0].position_.x = 2.0f * vertices[0].position_.x / constants::kResolution.x - 1.0f;
			vertices[1].position_.x = RotateX(_dst.x + _dst_size.x, _dst.y, _angle);
			vertices[1].position_.x = 2.0f * vertices[1].position_.x / constants::kResolution.x - 1.0f;
			vertices[2].position_.x = RotateX(_dst.x, _dst.y + _dst_size.y, _angle);
			vertices[2].position_.x = 2.0f * vertices[2].position_.x / constants::kResolution.x - 1.0f;
			vertices[3].position_.x = RotateX(_dst.x + _dst_size.x, _dst.y + _dst_size.y, _angle);
			vertices[3].position_.x = 2.0f * vertices[3].position_.x / constants::kResolution.x - 1.0f;

			vertices[0].position_.y = RotateY(_dst.x, _dst.y, _angle);
			vertices[0].position_.y = -2.0f * vertices[0].position_.y / constants::kResolution.y + 1.0f;
			vertices[1].position_.y = RotateY(_dst.x + _dst_size.x, _dst.y, _angle);
			vertices[1].position_.y = -2.0f * vertices[1].position_.y / constants::kResolution.y + 1.0f;
			vertices[2].position_.y = RotateY(_dst.x, _dst.y + _dst_size.y, _angle);
			vertices[2].position_.y = -2.0f * vertices[2].position_.y / constants::kResolution.y + 1.0f;
			vertices[3].position_.y = RotateY(_dst.x + _dst_size.x, _dst.y + _dst_size.y, _angle);
			vertices[3].position_.y = -2.0f * vertices[3].position_.y / constants::kResolution.y + 1.0f;

			vertices[0].position_.z = vertices[1].position_.z = vertices[2].position_.z = vertices[3].position_.z = 0.0f;
			vertices[0].color_ = vertices[1].color_ = vertices[2].color_ = vertices[3].color_ = _color;

			vertices[0].texcoord_.x = vertices[2].texcoord_.x = _src.x / tex_size_.x;
			vertices[1].texcoord_.x = vertices[3].texcoord_.x = _src.x + _src_size.x / tex_size_.x;
			vertices[0].texcoord_.y = vertices[1].texcoord_.y = _src.y / tex_size_.y;
			vertices[2].texcoord_.y = vertices[3].texcoord_.y = _src.y + _src_size.y / tex_size_.y;
		}

		memcpy(data, vertices, sizeof(vertices));
		vertex_buffer_->Unmap(0, nullptr);
	}
}