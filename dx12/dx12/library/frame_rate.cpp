/**
* @file FPSCounter.cpp
* @brief FPSCounterƒNƒ‰ƒX‚Ì’è‹`
*/
#include "frame_rate.h"
#include "constants.h"

#include <thread>
#include <sstream>

namespace snd::detail
{
	FrameRate::FrameRate(int _MaxFPS)
	{
		frame_interval_ = std::chrono::duration<double>((_MaxFPS == 0) ? 0.0 : 1.0 / _MaxFPS);
		frames_ = 0;
		currentFPS_ = 0;
		time_stamp_ = std::chrono::duration<double>(0.0);
		time_elapsed_ = std::chrono::duration<double>(0.0);
		last_time_ = std::chrono::high_resolution_clock::now();
	}

	void FrameRate::Run()
	{
		++frames_;

		std::chrono::high_resolution_clock::time_point current_time;
		std::chrono::duration<double> delta_time;
		std::chrono::duration<double> sleep_time;

		while (true)
		{
			current_time = std::chrono::high_resolution_clock::now();
			delta_time = std::chrono::duration(current_time - last_time_);
			sleep_time = frame_interval_ - delta_time;

			if (sleep_time.count() > 0.0)
			{
				std::this_thread::yield();
				continue;
			}
			break;
		}

		last_time_ = current_time;
		delta_time_ = delta_time;

		if ((time_stamp_ += delta_time).count() >= 1.0)
		{
			currentFPS_ = frames_;
			frames_ = 0;
			time_stamp_ = std::chrono::duration<double>(0.0);
		}
	}

	void FrameRate::ShowFPS(const HWND& _Hwnd)
	{
		std::ostringstream outs;
		outs.precision(4);
		outs << "FPS : " << currentFPS_ << " / " << "Frame Time : " << delta_time_.count() << " (ms)";
		SetWindowTextA(_Hwnd, outs.str().c_str());
	}
}
