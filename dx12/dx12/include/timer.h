#pragma once
#include <Windows.h>
#include <chrono>

#include "property.h"

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void Initialize(UINT _fps);
	void Update();

	Property<float> DeltaTime{ [this]() { return static_cast<float>(delta_time_.count()); }, nullptr };
	Property<UINT> FPS{ [this]() { return current_fps_; }, [this](UINT _fps) { frame_interval_ = std::chrono::duration<double>((_fps == 0) ? 0.0 : 1.0 / _fps); } };
private:
	std::chrono::high_resolution_clock::time_point last_time_ = {};
	std::chrono::duration<double>	delta_time_ = {};
	std::chrono::duration<double>	frame_interval_ = {};
	std::chrono::duration<double>	time_stamp_ = {};
	std::chrono::duration<double>	time_elapsed_ = {};
	UINT frame_count_ = 0;
	UINT current_fps_ = 0;
};