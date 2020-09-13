#pragma once
#include "../Misc/InputState.h"

namespace gl
{
	class Keyboard
	{
	private:
		InputState mInput;
	public:
		Keyboard() = default;
		~Keyboard() = default;
	public:
		void Initialize() {}
		bool State(Buttons _Key, Trigger _Trigger);
	};
}