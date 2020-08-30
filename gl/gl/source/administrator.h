#pragma once
#include "noncopyable.h"
#include "constants.h"

namespace gl
{
	class keyboard;
	class mouse;
	class xinput;

	class administrator : private noncopyable
	{
	private:
		inline static administrator* admin_ = nullptr;

		keyboard* keyboard_;
		mouse* mouse_;
		xinput* xinput_[kMaxControllerNum];
	public:
		administrator();
		~administrator();

		template<class T> [[nodiscard]] static T* get() { return nullptr; }
		template<class T> [[nodiscard]] static T* get(uint _index) { return nullptr; }

		template<> [[nodiscard]] static keyboard* get() { return admin_->keyboard_; }
		template<> [[nodiscard]] static mouse* get() { return admin_->mouse_; }
		template<> [[nodiscard]] static xinput* get(uint _index) { return admin_->xinput_[_index]; }
	};
}