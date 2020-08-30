#pragma once
#include <windows.h>

#include "mathematics.h"
#include "noncopyable.h"

namespace gl::detail
{
	class input_device;
}

namespace gl
{
	enum class trigger : uint
	{
		eKeep, ePress, eRelease
	};

	enum class buttons : int
	{
		eMouseLeft = 0x01, eMouseRight = 0x02, eMouseMiddle = 0x04,

		eKeyBackSpace = 0x05, eKeyTab = 0x09, eKeyEnter = 0x0d, eKeyEscape = 0x1b, eKeySpace = 0x20,
		eKeyLeft = 0x25, eKeyUp, eKeyRight, eKeyDown,
		eKey0 = 0x30, eKey1, eKey2, eKey3, eKey4, eKey5, eKey6, eKey7, eKey8, eKey9,
		eKeyA = 0x41, eKeyB, eKeyC, eKeyD, eKeyE, eKeyF, eKeyG, eKeyH, eKeyI, eKeyJ, eKeyK, eKeyL, eKeyM, eKeyN,
		eKeyO, eKeyP, eKeyQ, eKeyR, eKeyS, eKeyT, eKeyU, eKeyV, eKeyW, eKeyX, eKeyY, eKeyZ,
		eKeyF1 = 0x70, eKeyF2, eKeyF3, eKeyF4, eKeyF5, eKeyF6, eKeyF7, eKeyF8, eKeyF9, eKeyF10, eKeyF11, eKeyF12,
		eKeyLShift = 0xa0, eKeyRShift, eKeyLControl, eKeyRControl, eKeyLAlt, eKeyRAlt,

		eXInputA = 0x1000, eXInputB = 0x2000, eXInputX = 0x4000, eXInputY = 0x8000,
		eXInputUp = 0x0001, eXInputDown = 0x0002, eXInputLeft = 0x0004, eXInputRight = 0x0008,
		eXInputLThumb = 0x0040, eXInputRThumb = 0x0080,
		eXInputLShoulder = 0x0100, eXInputRShoulder = 0x0200,
		eXInputStart = 0x0010, eXInputSelect = 0x0020
	};
}

namespace gl::detail
{
	class input_device : private noncopyable
	{
	private:
		int current_state_ = 0;
		int previous_state_ = 0;

	public:
		input_device() = default;

		~input_device() = default;

		bool state(buttons _buttons, trigger _trigger)
		{
			previous_state_ = current_state_;
			if (GetAsyncKeyState(static_cast<int>(_buttons)) & 0x8000) current_state_++;
			else current_state_ = 0;

			if (_trigger == trigger::ePress) return previous_state_ == 0 && current_state_ > 0;
			else if (_trigger == trigger::eRelease) return previous_state_ > 0 && current_state_ == 0;
			else return current_state_ > 0;

			return false;
		}

		bool state(bool _is_xinput_button_press, trigger _trigger)
		{
			previous_state_ = current_state_;
			if (_is_xinput_button_press) current_state_++;
			else current_state_ = 0;

			if (_trigger == trigger::ePress) return previous_state_ == 0 && current_state_ > 0;
			else if (_trigger == trigger::eRelease) return previous_state_ > 0 && current_state_ == 0;
			else return current_state_ > 0;

			return false;
		}
	};
}