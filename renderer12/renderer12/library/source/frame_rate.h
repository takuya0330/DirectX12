/**
* @file FPSCounter.h
* @brief FPS管理クラス
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
		* @brief FPSの計測
		*/
		void Run();
		/**
		* @brief FPSの表示
		*/
		void ShowFPS(const HWND& _Hwnd);
		/**
		* @brief 最大FPSの再設定
		*/
		void SetMaxFPS(u_int _MaxFPS) { frame_interval_ = std::chrono::duration<double>((_MaxFPS == 0) ? 0.0 : 1.0 / _MaxFPS); }
	public:
		/**
		* @brief 現在のFPS取得
		*/
		u_int GetFPS()const noexcept { return currentFPS_; }
		/**
		* @brief 経過時間の取得
		*/
		float GetElapsedTime()const noexcept { return static_cast<float>(delta_time_.count()); }
	public:
		FrameRate(int _MaxFPS);
		~FrameRate() {}
	};
}
