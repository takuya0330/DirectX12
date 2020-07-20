#pragma once
#include <Windows.h>

#include "singleton.h"

namespace snd
{
	class D3D12Core;

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
			D3D12Core* GetGraphicsCore()const { return d3d12_core_; }
		private:
			friend SingletonHolder<Framework>;

			HWND hwnd_;
			D3D12Core* d3d12_core_;
		};
	}

	using System = SingletonHolder<detail::Framework>;
}