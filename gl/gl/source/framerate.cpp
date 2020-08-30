#include "framerate.h"

#include <thread>
#include <sstream>

namespace gl
{
	framerate::framerate(uint _max)
	{
		frame_interval_ = std::chrono::duration<double>((_max == 0) ? 0.0 : 1.0 / _max);
		frames_ = 0;
		current_fps_ = 0;
		time_stamp_ = std::chrono::duration<double>(0.0);
		time_elapsed_ = std::chrono::duration<double>(0.0);
		last_time_ = std::chrono::high_resolution_clock::now();
	}

	void framerate::run()
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
			current_fps_ = frames_;
			frames_ = 0;
			time_stamp_ = std::chrono::duration<double>(0.0);
		}
	}

	void framerate::show(const HWND& _Hwnd)
	{
		std::ostringstream outs;
		outs.precision(4);
		outs << "fps : " << current_fps_ << " / " << "frame time : " << delta_time_.count() << " (ms)";
		SetWindowTextA(_Hwnd, outs.str().c_str());
	}
}