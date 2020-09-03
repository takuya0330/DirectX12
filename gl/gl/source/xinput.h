#pragma once
#pragma comment (lib, "Xinput.lib")
#include <Windows.h>
#include <Xinput.h>
#include <array>

#include "input_device.h"
#include "mathematics.h"

namespace gl
{
	class xinput
	{
	private:
		static constexpr float kMaxTriggerValue = 255.0f;
		static constexpr float kMaxStickValue = 65535.0f;
		static constexpr float kHalfStickValue = 32767.0f;
		static constexpr uint kControllerNum = 4;

	private:
		struct controller
		{
			bool is_connect_ = false;
			float2 left_stick_ = 0;
			float2 right_stick_ = 0;
			float left_trigger_ = 0;
			float right_trigger_ = 0;
		};
		std::array<controller, kControllerNum> controllers_;
		std::array<detail::input_device, kControllerNum> input_devices_;

	private:
		bool is_connect(uint _index = 0)const noexcept { return controllers_[_index].is_connect_; }

	public:
		xinput() = default;
		~xinput() = default;

		void update(uint _index = 0)
		{
			_XINPUT_STATE xinput_state;

			controller& data = controllers_[_index];

			if (XInputGetState(static_cast<DWORD>(_index), &xinput_state) == ERROR_DEVICE_NOT_CONNECTED)
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

		bool state(buttons _buttons, trigger _trigger, uint _index = 0)
		{
			if (!is_connect(_index)) return false;

			_XINPUT_STATE xinput_state;
			auto& gamepad = xinput_state.Gamepad;

			bool is_press = (static_cast<int>(_buttons) & gamepad.wButtons) != 0;
			return input_devices_[_index].state(is_press, _trigger);
		}

		[[nodiscard]] float2 get_left_stick(float _dead_zone, uint _index = 0)const noexcept
		{
			if (!is_connect()) return 0;

			float2 value = controllers_[_index].left_stick_;
			if (math::abs(value.x) < _dead_zone) value.x = 0;
			if (math::abs(value.y) < _dead_zone) value.y = 0;

			return value;
		}
		[[nodiscard]] float2 get_right_stick(float _dead_zone, uint _index = 0)const noexcept
		{
			if (!is_connect()) return 0;

			float2 value = controllers_[_index].right_stick_;
			if (math::abs(value.x) < _dead_zone) value.x = 0;
			if (math::abs(value.y) < _dead_zone) value.y = 0;

			return value;
		}
		[[nodiscard]] float get_left_trigger(float _dead_zone, uint _index = 0)const noexcept
		{
			if (!is_connect()) return 0;

			float value = controllers_[_index].left_trigger_;
			if (math::abs(value) < _dead_zone) value = 0;

			return value;
		}
		[[nodiscard]] float get_right_trigger(float _dead_zone, uint _index = 0)const noexcept
		{
			if (!is_connect()) return 0;

			float value = controllers_[_index].right_trigger_;
			if (math::abs(value) < _dead_zone) value = 0;

			return value;
		}

		void begin_vibration(float _speed, uint _index = 0)const noexcept
		{
			if (!is_connect()) return;

			WORD value = static_cast<WORD>(_speed * kMaxStickValue);
			XINPUT_VIBRATION vibration = { value, value };
			XInputSetState(_index, &vibration);
		}
		void stop_vibration(uint _index = 0)const noexcept
		{
			XINPUT_VIBRATION vibration = { 0, 0 };
			XInputSetState(_index, &vibration);
		}
	};
}