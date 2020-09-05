#pragma comment (lib, "Xinput.lib")
#include <Windows.h>
#include <Xinput.h>

#include "XInput.h"

namespace gl
{
	void XInput::Update(uint _Index)
	{
		_XINPUT_STATE lState;

		Controller& lData = mControllers[_Index];

		if (XInputGetState(static_cast<DWORD>(_Index), &lState) == ERROR_DEVICE_NOT_CONNECTED)
		{
			lData.mIsConnect = false;
			return;
		}
		lData.mIsConnect = true;

		// -1 ~ 1 ‚Ì’l‚ÉŽû‚ß‚é
		auto& lPad = lState.Gamepad;
		lData.mLeftStick = float2(static_cast<float>(lPad.sThumbLX / kHalfStickValue), -static_cast<float>(lPad.sThumbLY / kHalfStickValue));
		lData.mRightStick = float2(static_cast<float>(lPad.sThumbRX / kHalfStickValue), -static_cast<float>(lPad.sThumbRY / kHalfStickValue));
		lData.mLeftTrigger = static_cast<float>(lPad.bLeftTrigger / kMaxTriggerValue);
		lData.mRightTrigger = static_cast<float>(lPad.bRightTrigger / kMaxTriggerValue);
	}

	bool XInput::State(uint _Index, Buttons _XInput, Trigger _Trigger)
	{
		if (!IsConnect(_Index)) return false;

		_XINPUT_STATE lState;
		auto& lPad = lState.Gamepad;
		Controller& lData = mControllers[_Index];

		return lData.mDevice.State((static_cast<int>(_XInput) & lPad.wButtons) != 0, _Trigger);
	}

	float2 XInput::GetLeftStick(uint _Index, float _DeadZone)const noexcept
	{
		if (!IsConnect(_Index)) return 0;

		float2 lValue = mControllers[_Index].mLeftStick;
		if (std::abs(lValue.x) < _DeadZone) lValue.x = 0;
		if (std::abs(lValue.y) < _DeadZone) lValue.y = 0;
		return lValue;
	}

	float2 XInput::GetRightStick(uint _Index, float _DeadZone)const noexcept
	{
		if (!IsConnect(_Index)) return 0;

		float2 lValue = mControllers[_Index].mRightStick;
		if (std::abs(lValue.x) < _DeadZone) lValue.x = 0;
		if (std::abs(lValue.y) < _DeadZone) lValue.y = 0;
		return lValue;
	}

	float XInput::GetLeftTrigger(uint _Index, float _DeadZone)const noexcept
	{
		if (!IsConnect(_Index)) return 0;

		float lValue = mControllers[_Index].mLeftTrigger;
		if (std::abs(lValue) < _DeadZone) lValue = 0;
		return lValue;
	}

	float XInput::GetRightTrigger(uint _Index, float _DeadZone)const noexcept
	{
		if (!IsConnect(_Index)) return 0;

		float lValue = mControllers[_Index].mLeftTrigger;
		if (std::abs(lValue) < _DeadZone) lValue = 0;
		return lValue;
	}

	void XInput::BeginVibration(uint _Index, float _Speed)const noexcept
	{
		if (!IsConnect(_Index)) return;

		WORD lValue = static_cast<WORD>(_Speed * kMaxStickValue);
		XINPUT_VIBRATION lVibration = { lValue, lValue };
		XInputSetState(_Index, &lVibration);
	}

	void XInput::StopVibration(uint _Index)const noexcept
	{
		XINPUT_VIBRATION lVibration = { 0, 0 };
		XInputSetState(_Index, &lVibration);
	}
}