#pragma once
#include <Windows.h>
#include <chrono>

#include "mathematics.h"

namespace gl
{
	class framerate
	{
	private:
		std::chrono::high_resolution_clock::time_point last_time_;
		std::chrono::duration<double>	delta_time_;
		std::chrono::duration<double>	frame_interval_;
		std::chrono::duration<double>	time_stamp_;
		std::chrono::duration<double>	time_elapsed_;
		uint frames_;
		uint current_fps_;

	public:
		framerate() = default;
		~framerate() = default;	
		framerate(uint _max);

		void run();	
		void show(const HWND& _hwnd);

		uint get_fps()const noexcept { return current_fps_; }	
		float get_elapsed_time()const noexcept { return static_cast<float>(delta_time_.count()); }

		void set_max_fps(uint _max) { frame_interval_ = std::chrono::duration<double>((_max == 0) ? 0.0 : 1.0 / _max); }
	};
}