#include "commands.h"
#include "utility.h"

namespace snd::detail
{
	void commands::create(ID3D12Device* _device)
	{
		// �R�}���h�L���[�̍쐬
		D3D12_COMMAND_QUEUE_DESC queue_desc = {};
		{
			queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;				// �R�}���h���X�g�ƍ��킹��
			queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// �v���C�I���e�B�͓��Ɏw��Ȃ�
			queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;				// �^�C���A�E�g����
			queue_desc.NodeMask = 0;												// �A�_�v�^�[���P�Ȃ�O�ł悵
		}
		ASSERT_SUCCEEDED(_device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_queue_)));
		utility::print("ID3D12CommandQueue create successed.");

		// �R�}���h�A���P�[�^�[�̍쐬
		ASSERT_SUCCEEDED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator_)));
		utility::print("ID3D12CommandAllocator create successed.");

		// �R�}���h���X�g�̍쐬
		ASSERT_SUCCEEDED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator_.Get(), nullptr, IID_PPV_ARGS(&graphics_command_list_)));
		utility::print("ID3D12GraphicsCommandList create successed.");
	}
}