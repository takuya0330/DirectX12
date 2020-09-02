#pragma once
#include <Windows.h>
#include "noncopyable.h"
#include "constants.h"

namespace gl
{
	class device;
	class device_context;
	class keyboard;
	class mouse;
	class xinput;

	class administrator : private noncopyable
	{
	private:
		inline static administrator* admin_ = nullptr;

		device* device_;
		device_context* device_context_;
		keyboard* keyboard_;
		mouse* mouse_;
		xinput* xinput_[kMaxControllerNum];
	public:
		administrator(HWND _hwnd);
		~administrator();

		template<class T> [[nodiscard]] static T* get() { return nullptr; }
		template<class T> [[nodiscard]] static T* get(uint _index) { return nullptr; }
		template<> [[nodiscard]] static device* get() { return admin_->device_; }
		template<> [[nodiscard]] static device_context* get() { return admin_->device_context_; }
		template<> [[nodiscard]] static keyboard* get() { return admin_->keyboard_; }
		template<> [[nodiscard]] static mouse* get() { return admin_->mouse_; }
		template<> [[nodiscard]] static xinput* get(uint _index) { return admin_->xinput_[_index]; }
	};
}