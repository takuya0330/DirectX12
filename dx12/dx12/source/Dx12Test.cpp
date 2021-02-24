#include "../header/Dx12Test.h"
#include "../header/utility.h"

using namespace Microsoft::WRL;

void Dx12Test::Initialize(HWND _hwnd)
{
	UINT dxgiflags = 0;
#if _DEBUG
	dxgiflags |= DXGI_CREATE_FACTORY_DEBUG;
	ComPtr<ID3D12Debug> debug_layer;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_layer))))
	{
		// �f�o�b�N���C���[�̗L����
		debug_layer->EnableDebugLayer();
	}
#endif
	ComPtr<IDXGIFactory2> factory2;
	ASSERT_SUCCEEDED(CreateDXGIFactory2(dxgiflags, IID_PPV_ARGS(&factory2)));
	factory2.As(&factory6);

	ComPtr<IDXGIAdapter1> adapter;
	DXGI_ADAPTER_DESC1 adapter_desc{};
	for (UINT i = 0; ; ++i)
	{
		if (factory6->EnumAdapters1(1, &adapter) != DXGI_ERROR_NOT_FOUND)
		{
			adapter->GetDesc1(&adapter_desc);
		}
		// �n�[�h�E�F�A�łȂ��ꍇ�͂�����x
		if (adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}
		// �쐬�\��
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	// �f�o�C�X�̍쐬
	ComPtr<ID3D12Device> device;
	ASSERT_SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
	device.As(&device5);

	// �R�}���h�L���[�̍쐬
	constexpr D3D12_COMMAND_QUEUE_DESC command_queue_desc =
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT, 				// ���ڃR�}���h���X�g�AGPU�̏�Ԃ��p�����Ȃ�
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,	// �w��Ȃ�
		.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE, 				// GPU�^�C���A�E�g�L��
		.NodeMask = 0 													// �P��A�_�v�^�̂���0
	};
	ASSERT_SUCCEEDED(device5->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&command_queue)));

	// �R�}���h�A���P�[�^�[�̐���
	ASSERT_SUCCEEDED(device5->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator)));
	
	// �R�}���h���X�g�̍쐬
	ASSERT_SUCCEEDED(device5->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator.Get(), nullptr, IID_PPV_ARGS(&command_list)));
	command_list->Close();

	// �X���b�v�`�F�C���̍쐬
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc =
	{
		.Width = kWindowWidth,
		.Height = kWindowHeight,
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.Stereo = false,											// �t���X�N���[���\�����[�h���o�b�N�o�b�t�@���X�e���I�ɂ��邩�ǂ������w�肷��
		.SampleDesc =
		{
			.Count = 1,
			.Quality = 0
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = kFrameBufferCount,
		.Scaling = DXGI_SCALING_STRETCH,						// �o�b�N�o�b�t�@�̓��e���v���[���e�[�V�����̃^�[�Q�b�g�T�C�Y�ɍ��킹��
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,		// Present1���Ăяo������A�o�b�N�o�b�t�@�̓��e��j������悤�w�肷��
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,			// ���ߓ�����w�肵�Ȃ�
		//.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH // �E�B���h�E���[�h�ƃt���X�N���[�����[�h�̐؂�ւ����\
	};

	ComPtr<IDXGISwapChain1> swap_chain1;
	ASSERT_SUCCEEDED(factory6->CreateSwapChainForHwnd(command_queue.Get(), _hwnd, &swap_chain_desc, nullptr, nullptr, &swap_chain1));
	swap_chain1.As(&swap_chain4);

	// ALT+ENTER�֎~
	factory6->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER);

	// �f�B�X�N���v�^�q�[�v(RTV)�̍쐬
	constexpr D3D12_DESCRIPTOR_HEAP_DESC RTV_heap_desc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,	// �����_�[�^�[�Q�b�g�r���[�쐬�p
		.NumDescriptors = kFrameBufferCount,		// �f�B�X�N���v�^�̐�
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE, // �f�t�H���g�̎g�p�@������
		.NodeMask = 0										// �P��A�_�v�^�̂���0
	};
	ASSERT_SUCCEEDED(device5->CreateDescriptorHeap(&RTV_heap_desc, IID_PPV_ARGS(&RTV_heap)));
	RTV_heap_size = device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// �����_�[�^�[�Q�b�g�r���[�̍쐬
	D3D12_CPU_DESCRIPTOR_HANDLE RTV_handle = RTV_heap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < kFrameBufferCount; ++i)
	{
		ASSERT_SUCCEEDED(swap_chain4->GetBuffer(i, IID_PPV_ARGS(&back_buffer.at(i))));

		device5->CreateRenderTargetView(back_buffer.at(i).Get(), nullptr, RTV_handle);
		RTV_handle.ptr += RTV_heap_size;
	}

	// �f�B�X�N���v�^�q�[�v(DSV)�̍쐬
	constexpr D3D12_DESCRIPTOR_HEAP_DESC DSV_heap_desc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,	// �����_�[�^�[�Q�b�g�r���[�쐬�p
		.NumDescriptors = 1,								// �f�B�X�N���v�^�̐�
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE, // �f�t�H���g�̎g�p�@������
		.NodeMask = 0										// �P��A�_�v�^�̂���0
	};
	ASSERT_SUCCEEDED(device5->CreateDescriptorHeap(&DSV_heap_desc, IID_PPV_ARGS(&DSV_heap)));

	// �f�v�X�o�b�t�@�̍쐬
	constexpr D3D12_HEAP_PROPERTIES heap_properties =
	{
		.Type = D3D12_HEAP_TYPE_DEFAULT,							// CPU�A�N�Z�X��񋟂ł��Ȃ��AGPU�̓v�[�����烁�����ւ̓ǂݏ������\
		.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,// �q�[�v��CPU�y�[�W�v���p�e�B�͕s��
		.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,	// �q�[�v�̃������v�[���͕s��
		.CreationNodeMask = 1,										// 0,1�ǂ���ł�����
		.VisibleNodeMask = 1											// CreationNodeMask�Ɠ����l�ł���K�v������
	};
	constexpr D3D12_RESOURCE_DESC resource_desc =
	{
		.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,	// Texture2D�Ƃ��Ďg�p����
		.Alignment = 0,												// �A���C�����g�̎w��
		.Width = kWindowWidth,										// ��ʉ���
		.Height = kWindowHeight,									// ��ʏc��
		.DepthOrArraySize = 1,										// �z��T�C�Y�̎w��
		.MipLevels = 0,												// �~�b�v���x���̐��̎w��
		.Format = DXGI_FORMAT_D32_FLOAT,							// �t�H�[�}�b�g�̎w��
		.SampleDesc =
		{
			.Count = 1,													// ��f������̃}���`�T���v����
			.Quality = 0												// �掿���x���A�掿�������قǃp�t�H�[�}���X���ቺ����
		},
		.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,				// �s��
		.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL	// �f�v�X�X�e���V���r���[���쐬�ł���悤�Ɏw��
	};
	constexpr D3D12_CLEAR_VALUE clear_value =
	{
		.Format = DXGI_FORMAT_D32_FLOAT, // ���\�[�X�ƍ��킹��
		.DepthStencil =
		{
			.Depth = 1.0f,						// �[�x�l
			.Stencil = 0						// �X�e���V���l
		}
	};
	ASSERT_SUCCEEDED(device5->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear_value, IID_PPV_ARGS(&depth_buffer)));

	// �f�v�X�X�e���V���r���[�̍쐬
	constexpr D3D12_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc =
	{
		.Format = DXGI_FORMAT_D32_FLOAT,						// ���\�[�X�ƍ��킹��
		.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D, // �f�v�X�X�e���V�����\�[�X���ǂ̂悤�ɃA�N�Z�X����邩���w�肷��
		.Flags = D3D12_DSV_FLAG_NONE,							// �ǂݎ���p�ł͂Ȃ����Ƃ��w�肷��
		.Texture2D =
		{
			.MipSlice = 0											// �ŏ��Ɏg�p����~�b�v�}�b�v���x���̃C���f�b�N�X
		}
	};
	device5->CreateDepthStencilView(depth_buffer.Get(), &depth_stencil_view_desc, DSV_heap->GetCPUDescriptorHandleForHeapStart());

	// �t�F���X�̍쐬
	ASSERT_SUCCEEDED(device5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	fence_event = CreateEvent(nullptr, false, false, nullptr);
	fence_value = 0;

	// �r���[�|�[�g�̐ݒ�
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(kWindowWidth);
	viewport.Height = static_cast<float>(kWindowHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	scissor_rect.left = 0;
	scissor_rect.top = 0;
	scissor_rect.right = static_cast<LONG>(kWindowWidth);
	scissor_rect.bottom = static_cast<LONG>(kWindowHeight);
}

void Dx12Test::RenderBegin()
{
	// ���݂̃o�b�N�o�b�t�@�̃C���f�b�N�X�擾
	frame_buffer_index = swap_chain4->GetCurrentBackBufferIndex();

	// �R�}���h�̃��Z�b�g
	command_allocator->Reset();
	command_list->Reset(command_allocator.Get(), nullptr);

	// ���\�[�X�o���A�̐ݒ�
	const D3D12_RESOURCE_BARRIER barrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = back_buffer.at(frame_buffer_index).Get(),
			.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			.StateBefore = D3D12_RESOURCE_STATE_PRESENT,
			.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET
		}
	};
	command_list->ResourceBarrier(1, &barrier);

	// ��ʂ̃N���A
	D3D12_CPU_DESCRIPTOR_HANDLE RTV_handle = RTV_heap->GetCPUDescriptorHandleForHeapStart();
	RTV_handle.ptr += static_cast<size_t>(frame_buffer_index) * static_cast<size_t>(RTV_heap_size);
	D3D12_CPU_DESCRIPTOR_HANDLE DSV_handle = DSV_heap->GetCPUDescriptorHandleForHeapStart();
	command_list->OMSetRenderTargets(1, &RTV_handle, false, &DSV_handle);
	command_list->ClearRenderTargetView(RTV_handle, kClearColor, 0, nullptr);
	command_list->ClearDepthStencilView(DSV_handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// �r���[�|�[�g�̃Z�b�g
	command_list->RSSetViewports(1, &viewport);
	command_list->RSSetScissorRects(1, &scissor_rect);
}

void Dx12Test::RenderEnd()
{
	// ���\�[�X�o���A�̐ݒ�
	const D3D12_RESOURCE_BARRIER barrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = back_buffer.at(frame_buffer_index).Get(),
			.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
			.StateAfter = D3D12_RESOURCE_STATE_PRESENT
		}
	};
	command_list->ResourceBarrier(1, &barrier);

	// �R�}���h�̐ςݍ��ݏI��
	command_list->Close();

	// �R�}���h���X�g�̎��s
	ID3D12CommandList* execute_list[] = { command_list.Get() };
	command_queue->ExecuteCommandLists(1, execute_list);

	swap_chain4->Present(0, 0);

	// GPU�̏����������������m�F����
	WaitFence();
}

void Dx12Test::WaitFence()
{
	command_queue->Signal(fence.Get(), ++fence_value);
	if (fence->GetCompletedValue() < fence_value)
	{
		fence->SetEventOnCompletion(fence_value, fence_event);
		WaitForSingleObject(fence_event, INFINITE);
	}
}