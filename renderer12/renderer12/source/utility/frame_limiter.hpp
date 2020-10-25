#pragma once
#include <Windows.h>
#include <chrono>

namespace re12::detail
{
	class FrameLimiter
	{
	private:
		std::chrono::high_resolution_clock::time_point mLastTime;
		std::chrono::duration<double>	mDeltaTime;
		std::chrono::duration<double>	mFrameInterval;
		std::chrono::duration<double>	mTimeStamp;
		std::chrono::duration<double>	mTimeElapsed;
		UINT mFrames;
		UINT mCurrentFPS;
	public:
		FrameLimiter() = default;
		~FrameLimiter() = default;
		FrameLimiter(UINT _MaxFPS);
	public:
		void Update();
		void Show(const HWND& _Hwnd);
	public:
		UINT GetFPS()const noexcept { return mCurrentFPS; }
		float GetElapsedTime()const noexcept { return static_cast<float>(mDeltaTime.count()); }
	public:
		void SetMaxFPS(UINT _MaxFPS) { mFrameInterval = std::chrono::duration<double>((_MaxFPS == 0) ? 0.0 : 1.0 / _MaxFPS); }
	};
}