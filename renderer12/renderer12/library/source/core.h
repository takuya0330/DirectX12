#pragma once
#include <Windows.h>

#include "singleton.h"

namespace snd
{
	class Device;
	class Renderer;

	namespace detail
	{
		class System
		{
		public:
			System() {}
			~System();

			void Initialize(HWND _hwnd);
			bool Update();

			HWND GetHWND()const { return hwnd_; }
			Device* GetDevice()const { return device_; }
			Renderer* GetRenderer()const { return renderer_; }
		private:
			friend SingletonHolder<System>;

			HWND hwnd_;
			Device* device_;
			Renderer* renderer_;
		};
	}

	using Core = SingletonHolder<detail::System>;
}