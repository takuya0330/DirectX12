/**
* @file FPSCounter.h
* @brief FPS�Ǘ��N���X
*/
#pragma once
#include <Windows.h>
#include <chrono>

namespace snd
{
	class FrameRate
	{
	private:
		std::chrono::high_resolution_clock::time_point last_time_;
		std::chrono::duration<double>						  delta_time_;
		std::chrono::duration<double>						  frame_interval_;
		std::chrono::duration<double>						  time_stamp_;
		std::chrono::duration<double>						  time_elapsed_;
		u_int frames_;
		u_int currentFPS_;
	public:
		/**
		* @brief FPS�̌v��
		*/
		void Run();
		/**
		* @brief FPS�̕\��
		*/
		void ShowFPS(const HWND& _Hwnd);
		/**
		* @brief �ő�FPS�̍Đݒ�
		*/
		void SetMaxFPS(u_int _MaxFPS) { frame_interval_ = std::chrono::duration<double>((_MaxFPS == 0) ? 0.0 : 1.0 / _MaxFPS); }
	public:
		/**
		* @brief ���݂�FPS�擾
		*/
		u_int GetFPS()const noexcept { return currentFPS_; }
		/**
		* @brief �o�ߎ��Ԃ̎擾
		*/
		float GetElapsedTime()const noexcept { return static_cast<float>(delta_time_.count()); }
	public:
		FrameRate(int _MaxFPS);
		~FrameRate() {}
	};
}
