#include <Windows.h>

#include "keyboard.hpp"

namespace re12::detail
{
	bool Keyboard::State(Buttons _Key, Trigger _Trigger)
	{
		return mInput.State(GetAsyncKeyState(static_cast<int>(_Key)) & 0x8000, _Trigger);
	}
}