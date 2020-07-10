#include "commands.h"
#include "utility.h"

namespace snd::detail
{
	void commands::create(ID3D12Device* _device)
	{
		// コマンドキューの作成
		D3D12_COMMAND_QUEUE_DESC queue_desc = {};
		{
			queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;				// コマンドリストと合わせる
			queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// プライオリティは特に指定なし
			queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;				// タイムアウト無し
			queue_desc.NodeMask = 0;												// アダプターが１つなら０でよし
		}
		ASSERT_SUCCEEDED(_device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_queue_)));
		utility::print("ID3D12CommandQueue create successed.");

		// コマンドアロケーターの作成
		ASSERT_SUCCEEDED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator_)));
		utility::print("ID3D12CommandAllocator create successed.");

		// コマンドリストの作成
		ASSERT_SUCCEEDED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator_.Get(), nullptr, IID_PPV_ARGS(&graphics_command_list_)));
		utility::print("ID3D12GraphicsCommandList create successed.");
	}
}