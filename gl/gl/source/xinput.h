#pragma once
#pragma comment (lib, "Xinput.lib")
#include <Windows.h>
#include <Xinput.h>

#include "input_device.h"
#include "constants.h"

namespace gl
{
	class xinput
	{
	private:
		static constexpr float kMaxTriggerValue = 255.0f;
		static constexpr float kMaxStickValue = 65535.0f;
		static constexpr float kHalfStickValue = 32767.0f;

	private:
		detail::input_device input_device_;
		uint controller_index_ = 0;
		struct controller
		{
			bool is_connect_ = false;
			float2 left_stick_ = 0;
			float2 right_stick_ = 0;
			float left_trigger_ = 0;
			float right_trigger_ = 0;
		} controller_[kMaxControllerNum];

	private:
		bool is_connect()const noexcept { return controller_[controller_index_].is_connect_; }

	public:
		xinput() = default;

		xinput(uint _index) : controller_index_(_index) {}

		void update()
		{
			_XINPUT_STATE xinput_state;

			controller& data = controller_[controller_index_];

			if (XInputGetState(static_cast<DWORD>(controller_index_), &xinput_state) == ERROR_DEVICE_NOT_CONNECTED)
			{
				data.is_connect_ = false;
				return;
			}
			data.is_connect_ = true;

			auto& gamepad = xinput_state.Gamepad;
			data.left_stick_ = float2(static_cast<float>(gamepad.sThumbLX / kHalfStickValue), -static_cast<float>(gamepad.sThumbLY / kHalfStickValue));
			data.right_stick_ = float2(static_cast<float>(gamepad.sThumbRX / kHalfStickValue), -static_cast<float>(gamepad.sThumbRY / kHalfStickValue));
			data.left_trigger_ = static_cast<float>(gamepad.bLeftTrigger / kMaxTriggerValue);
			data.right_trigger_ = static_cast<float>(gamepad.bRightTrigger / kMaxTriggerValue);
		}

		bool state(buttons _buttons, trigger _trigger)
		{
			if (!is_connect()) return false;

			_XINPUT_STATE xinput_state;
			auto& gamepad = xinput_state.Gamepad;

			bool is_press = (static_cast<int>(_buttons) & gamepad.wButtons) != 0;
			return input_device_.state(is_press, _trigger);
		}

		float2 get_left_stick(float _dead_zone)const noexcept
		{
			if (!is_connect()) return 0;

			float2 value = controller_[controller_index_].left_stick_;
			if (math::abs(value) < _dead_zone) value = 0;

			return value;
		}

		float2 get_right_stick(float _dead_zone)const noexcept
		{
			if (!is_connect()) return 0;

			float2 value = controller_[controller_index_].right_stick_;
			if (math::abs(value) < _dead_zone) value = 0;

			return value;
		}

		float get_left_trigger(float _dead_zone)const noexcept
		{
			if (!is_connect()) return 0;

			float value = controller_[controller_index_].left_trigger_;
			if (math::abs(value) < _dead_zone) value = 0;

			return value;
		}

		float get_right_trigger(float _dead_zone)const noexcept
		{
			if (!is_connect()) return 0;

			float value = controller_[controller_index_].right_trigger_;
			if (math::abs(value) < _dead_zone) value = 0;

			return value;
		}

		void begin_vibration(float _speed)const noexcept
		{
			if (!is_connect()) return;

			WORD value = static_cast<WORD>(_speed * kMaxStickValue);
			XINPUT_VIBRATION vibration = { value, value };
			XInputSetState(controller_index_, &vibration);
		}

		void stop_vibration()const noexcept
		{
			XINPUT_VIBRATION vibration = { 0, 0 };
			XInputSetState(controller_index_, &vibration);
		}
	};
}