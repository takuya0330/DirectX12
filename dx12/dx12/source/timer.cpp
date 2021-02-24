#include <thread>
#include <sstream>

#include "../header/timer.h"

void Timer::Initialize(UINT _maxFPS)
{
	last_time = std::chrono::high_resolution_clock::now();
	delta_time = std::chrono::duration<double>(0.0);
	frame_interval = std::chrono::duration<double>((_maxFPS == 0) ? 0.0 : 1.0 / _maxFPS);
	time_stamp = std::chrono::duration<double>(0.0);
	time_elapsed = std::chrono::duration<double>(0.0);
	frames = 0;
	currentFPS = 0;
}
void Timer::Update()
{
	++frames;

	std::chrono::high_resolution_clock::time_point current;
	std::chrono::duration<double> delta;
	std::chrono::duration<double> sleep;

	while (true)
	{
		current = std::chrono::high_resolution_clock::now();
		delta = std::chrono::duration(current - last_time);
		sleep = frame_interval - delta;

		if (sleep.count() > 0.0)
		{
			std::this_thread::yield();
			continue;
		}
		break;
	}

	last_time = current;
	delta_time = delta;

	if ((time_stamp += delta).count() >= 1.0)
	{
		currentFPS = frames;
		frames = 0;
		time_stamp = std::chrono::duration<double>(0.0);
	}
}
void Timer::Show(const HWND& _hwnd)
{
	std::ostringstream outs;
	outs.precision(4);
	outs << "FPS : " << currentFPS << " / " << "Frame Time : " << delta_time.count() << " (ms)";
	SetWindowTextA(_hwnd, outs.str().c_str());
}
