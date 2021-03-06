#include <thread>

#include "../include/timer.h"

void Timer::Initialize(UINT _fps)
{
	last_time_ = std::chrono::high_resolution_clock::now();
	delta_time_ = std::chrono::duration<double>(0.0);
	frame_interval_ = std::chrono::duration<double>((_fps == 0) ? 0.0 : 1.0 / _fps);
	time_stamp_ = std::chrono::duration<double>(0.0);
	time_elapsed_ = std::chrono::duration<double>(0.0);
	frame_count_ = 0;
	current_fps_ = 0;
}

void Timer::Update()
{
	++frame_count_;

	std::chrono::high_resolution_clock::time_point current_time;
	std::chrono::duration<double> elapsed_time;
	std::chrono::duration<double> sleep_time;

	while (true)
	{
		// ���݂̎��Ԃ��擾
		current_time = std::chrono::high_resolution_clock::now();
		// �o�ߎ��Ԃ��Z�o
		elapsed_time = std::chrono::duration(current_time - last_time_);
		// �ҋ@���Ԃ��Z�o
		sleep_time = frame_interval_ - elapsed_time;

		// �ҋ@
		if (sleep_time.count() > 0.0)
		{
			std::this_thread::yield();
			continue;
		}
		break;
	}

	last_time_ = current_time;
	delta_time_ = elapsed_time;

	if ((time_stamp_ += elapsed_time).count() >= 1.0)
	{
		current_fps_ = frame_count_;
		frame_count_ = 0;
		time_stamp_ = std::chrono::duration<double>(0.0);
	}
}