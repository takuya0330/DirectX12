#include <thread>
#include <sstream>

#include "FrameLimiter.h"

FrameLimiter::FrameLimiter(uint _MaxFPS)
{
	mLastTime = std::chrono::high_resolution_clock::now();
	mDeltaTime = std::chrono::duration<double>(0.0);
	mFrameInterval = std::chrono::duration<double>((_MaxFPS == 0) ? 0.0 : 1.0 / _MaxFPS);
	mTimeStamp = std::chrono::duration<double>(0.0);
	mTimeElapsed = std::chrono::duration<double>(0.0);
	mFrames = 0;
	mCurrentFPS = 0;
}

void FrameLimiter::Update()
{
	++mFrames;

	std::chrono::high_resolution_clock::time_point lCurrentTime;
	std::chrono::duration<double> lDeltaTime;
	std::chrono::duration<double> lSleepTime;

	while (true)
	{
		lCurrentTime = std::chrono::high_resolution_clock::now();
		lDeltaTime = std::chrono::duration(lCurrentTime - mLastTime);
		lSleepTime = mFrameInterval - lDeltaTime;

		if (lSleepTime.count() > 0.0)
		{
			std::this_thread::yield();
			continue;
		}
		break;
	}

	mLastTime = lCurrentTime;
	mDeltaTime = lDeltaTime;

	if ((mTimeStamp += lDeltaTime).count() >= 1.0)
	{
		mCurrentFPS = mFrames;
		mFrames = 0;
		mTimeStamp = std::chrono::duration<double>(0.0);
	}
}

void FrameLimiter::Show(const HWND& _Hwnd)
{
	std::ostringstream outs;
	outs.precision(4);
	outs << "FPS : " << mCurrentFPS << " / " << "Frame Time : " << mDeltaTime.count() << " (ms)";
	SetWindowTextA(_Hwnd, outs.str().c_str());
}
