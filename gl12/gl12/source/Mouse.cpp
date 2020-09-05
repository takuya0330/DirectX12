#include <Windows.h>

#include "Mouse.h"

namespace gl
{
	void Mouse::Initialize()
	{
		mPosition = 0;
		mDrag = 0;
		mMove = 0;
		mWheel = 0;
		mScroll = 0;
	}

	void Mouse::Update()
	{
		// マウスポインターの位置取得
		POINT lPoint;
		GetCursorPos(&lPoint);
		ScreenToClient(GetActiveWindow(), &lPoint);

		//	移動量取得
		const int2 lPosition = int2(static_cast<int>(lPoint.x), static_cast<int>(lPoint.y));
		mMove = int2(lPosition.x - mPosition.x, lPosition.y - mPosition.y);

		mPosition = lPosition;
		mWheel = mScroll;
		mScroll = 0;
	}

	bool Mouse::State(Buttons _Mouse, Trigger _Trigger)
	{
		return mDevice.State(GetAsyncKeyState(static_cast<int>(_Mouse)) & 0x8000, _Trigger);
	}
}