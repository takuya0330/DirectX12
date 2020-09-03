#pragma once
#include "input_device.h"

namespace gl
{
	class mouse
	{
	private:
		detail::input_device input_device_;
		int2 position_;
		int2 drag_;
		int2 move_;
		int wheel_ = 0;
		int scroll_ = 0;

	public:
		mouse() = default;
		~mouse() = default;

		void update()
		{
			// マウスポインターの位置取得
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(GetActiveWindow(), &point);

			const int2 current = int2(static_cast<int>(point.x), static_cast<int>(point.y));
			move_ = int2(current.x - position_.x, current.y - position_.y);

			position_ = current;
			wheel_ = scroll_;
			scroll_ = 0;
		}

		bool state(buttons _buttons, trigger _trigger) { return input_device_.state((GetAsyncKeyState(static_cast<int>(_buttons)) & 0x8000), _trigger); }

		[[nodiscard]] int2 get_pos()const noexcept { return position_; }
		[[nodiscard]] int2 get_drag()const noexcept { return drag_; }
		[[nodiscard]] int2 get_move()const noexcept { return move_; }
		[[nodiscard]] int get_wheel()const noexcept { return wheel_; }

		void set_pos(const int2& _position)noexcept { position_ = _position; }
		void set_drag(const int2& _drag)noexcept { drag_ = _drag; }

		void add_scroll(int _delta)noexcept { scroll_ += _delta; }
	};
}