#pragma once
#include "../utility/input_state.hpp"

namespace re12::detail
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