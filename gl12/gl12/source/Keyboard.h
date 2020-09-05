#pragma once
#include "InputDevice.h"

namespace gl
{
	class Keyboard
	{
	private:
		detail::InputDevice mDevice;
	public:
		Keyboard() = default;
		~Keyboard() = default;
	public:
		void Initialize() {}
		bool State(Buttons _Key, Trigger _Trigger);
	};
}