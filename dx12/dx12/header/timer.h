#pragma once
#include <Windows.h>
#include <chrono>
//#include <thread>
//#include <sstream>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;
public:
	void Initialize(UINT _maxFPS = 60);
	void Update();
	void Show(const HWND& _hwnd);
public:
	void SetMaxFPS(UINT _maxFPS) { frame_interval = std::chrono::duration<double>((_maxFPS == 0) ? 0.0 : 1.0 / _maxFPS); }
private:
	std::chrono::high_resolution_clock::time_point last_time;
	std::chrono::duration<double>	delta_time;
	std::chrono::duration<double>	frame_interval;
	std::chrono::duration<double>	time_stamp;
	std::chrono::duration<double>	time_elapsed;
	UINT frames;
	UINT currentFPS;
};