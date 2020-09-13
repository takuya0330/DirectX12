#pragma once
#include <Windows.h>
#include <chrono>

#include "../Mathematics.h"

class FrameLimiter
{
private:
	std::chrono::high_resolution_clock::time_point mLastTime;
	std::chrono::duration<double>	mDeltaTime;
	std::chrono::duration<double>	mFrameInterval;
	std::chrono::duration<double>	mTimeStamp;
	std::chrono::duration<double>	mTimeElapsed;
	uint mFrames;
	uint mCurrentFPS;
public:
	FrameLimiter() = default;
	~FrameLimiter() = default;
	FrameLimiter(uint _MaxFPS);
public:
	void Update();
	void Show(const HWND& _Hwnd);
public:
	uint GetFPS()const noexcept { return mCurrentFPS; }
	float GetElapsedTime()const noexcept { return static_cast<float>(mDeltaTime.count()); }
public:
	void SetMaxFPS(uint _MaxFPS) { mFrameInterval = std::chrono::duration<double>((_MaxFPS == 0) ? 0.0 : 1.0 / _MaxFPS); }
};
