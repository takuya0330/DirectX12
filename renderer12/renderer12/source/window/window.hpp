#pragma once
#include <Windows.h>

namespace re12::detail
{
	class Window
	{
	private:
		// �^�C�g���o�[�\�����e
		static constexpr const char* kTitle = "Renderer12";

		// exe�N�����ɂ�����J�[�\���\���̗L��
		static constexpr bool kShowCursor = true;
	private:
		HWND mHwnd = {};
	public:
		// �E�B���h�E�T�C�Y�i�[�ϐ�
		struct
		{
			UINT mWidth = 0;
			UINT mHeight = 0;
		} mSize;
	public:
		Window() = default;
		~Window() = default;
	public:
		// ������
		void Initialize(UINT _Width, UINT _Height);
		
		// �Q�[�����[�v�X�V
		bool Update();

		// �E�B���h�E�\��
		void Show(int _CmdShow);
	public:
		HWND GetHwnd()noexcept { return mHwnd; }
	};
}