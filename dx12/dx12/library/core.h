#pragma once
#include <Windows.h>

#include "singleton.h"

namespace snd
{
	namespace detail
	{
		class Device;
		class Renderer;

		class System
		{
		public:
			System() {}
			~System();

			void Initialize(HWND _hwnd);
			bool Update();

			Device* GetDevice()const { return device_; }
			Renderer* GetRenderer()const { return renderer_; }
		private:
			friend SingletonHolder<System>;

			Device* device_;
			Renderer* renderer_;
		};
	}

	using Core = SingletonHolder<detail::System>;
}
