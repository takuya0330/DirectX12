#pragma once
#include "../utility/input_state.hpp"
#include "../math/mathmatics.hpp"

namespace re12::detail
{
	class Mouse
	{
	private:
		InputState mInput;
		int2 mPosition;
		int2 mDrag;
		int2 mMove;
		int mWheel = 0;
		int mScroll = 0;
	public:
		Mouse() = default;
		~Mouse() = default;
	public:
		void Initialize();
		void Update();
		bool State(Buttons _Mouse, Trigger _Trigger);
	public:
		int2 GetPos()const noexcept { return mPosition; }
		int2 GetDrag()const noexcept { return mDrag; }
		int2 GetMove()const noexcept { return mMove; }
		int GetWheel()const noexcept { return mWheel; }
	public:
		void SetPos(const int2& _Position)noexcept { mPosition = _Position; }
		void SetDrag(const int2& _Drag)noexcept { mDrag = _Drag; }
		void AddScroll(int _Delta)noexcept { mScroll += _Delta; }
	};
}