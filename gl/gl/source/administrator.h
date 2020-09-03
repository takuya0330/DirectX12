#pragma once
#include <Windows.h>
#include "noncopyable.h"
#include "constants.h"

namespace gl
{
	class generate_device;
	class rendering_device;
	class keyboard;
	class mouse;
	class xinput;

	template<class T>
	class component
	{
	private:
		T* ptr_ = nullptr;
	public:
		component() { ptr_ = new T(); }

		void release()
		{
			delete ptr_;
			ptr_ = nullptr;
		}

		T* get() { return ptr_; }
		T* operator->() { return ptr_; }
	};

	class administrator : private noncopyable
	{
	private:
		inline static administrator* admin_ = nullptr;

		component<generate_device> generate_device_;
		component<rendering_device> rendering_device_;
		component<keyboard> keyboard_;
		component<mouse> mouse_;
		component<xinput> xinput_;
	public:
		administrator(HWND _hwnd);
		~administrator();

		template<class T> [[nodiscard]] static T* get() { return nullptr; }
		template<> [[nodiscard]] static generate_device* get() { return admin_->generate_device_.get(); }
		template<> [[nodiscard]] static rendering_device* get() { return admin_->rendering_device_.get(); }
		template<> [[nodiscard]] static keyboard* get() { return admin_->keyboard_.get(); }
		template<> [[nodiscard]] static mouse* get() { return admin_->mouse_.get(); }
		template<> [[nodiscard]] static xinput* get() { return admin_->xinput_.get(); }
	};
}