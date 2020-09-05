#include <Windows.h>

#include "Keyboard.h"

namespace gl
{
	bool Keyboard::State(Buttons _Key, Trigger _Trigger)
	{
		return mDevice.State(GetAsyncKeyState(static_cast<int>(_Key)) & 0x8000, _Trigger);
	}
}