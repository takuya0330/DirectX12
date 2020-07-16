#pragma once
#include <Windows.h>

#include "singleton.h"

namespace snd
{
	class GraphicsCore;

	namespace detail
	{
		class Framework
		{
		public:
			Framework() {}
			~Framework();

			void Initialize(HWND _hwnd);
			bool Update();

			HWND GetHWND()const { return hwnd_; }
			GraphicsCore* GetGraphicsCore()const { return graphics_core_; }
		private:
			friend SingletonHolder<Framework>;

			HWND hwnd_;
			GraphicsCore* graphics_core_;
		};
	}

	using System = SingletonHolder<detail::Framework>;
}