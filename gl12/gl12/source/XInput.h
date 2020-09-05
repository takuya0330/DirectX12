#pragma once
#include <array>

#include "InputDevice.h"
#include "Mathematics.h"

namespace gl
{
	class XInput
	{
	private:
		static constexpr float kMaxTriggerValue = 255.0f;
		static constexpr float kMaxStickValue = 65535.0f;
		static constexpr float kHalfStickValue = 32767.0f;
		static constexpr uint kControllerNum = 4;
	private:
		struct Controller
		{
			detail::InputDevice mDevice;
			float2 mLeftStick;
			float2 mRightStick;
			float	mLeftTrigger = 0;
			float	mRightTrigger = 0;
			bool mIsConnect = false;
		};
		std::array<Controller, kControllerNum> mControllers;
	private:
		bool IsConnect(uint _Index)const noexcept { return mControllers[_Index].mIsConnect; }
	public:
		XInput() = default;
		~XInput() = default;
	public:
		void Initialize() {}
		void Update(uint _Index);
		bool State(uint _Index, Buttons _XInput, Trigger _Trigger);
	public:
		float2 GetLeftStick(uint _Index, float _DeadZone)const noexcept;
		float2 GetRightStick(uint _Index, float _DeadZone)const noexcept;
		float GetLeftTrigger(uint _Index, float _DeadZone)const noexcept;
		float GetRightTrigger(uint _Index, float _DeadZone)const noexcept;
	public:
		void BeginVibration(uint _Index, float _Speed)const noexcept;
		void StopVibration(uint _Index)const noexcept;
	};
}