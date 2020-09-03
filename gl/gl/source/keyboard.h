#pragma once
#include "input_device.h"

namespace gl
{
	class keyboard
	{
	private:
		detail::input_device input_device_;

	public:
		keyboard() = default;
		~keyboard() = default;

		bool state(buttons _buttons, trigger _trigger) { return input_device_.state((GetAsyncKeyState(static_cast<int>(_buttons)) & 0x8000), _trigger); }
	};
}