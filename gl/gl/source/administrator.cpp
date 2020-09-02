#include "administrator.h"
#include "device.h"
#include "device_context.h"
#include "keyboard.h"
#include "mouse.h"
#include "xinput.h"

namespace gl
{
	administrator::administrator(HWND _hwnd)
	{
		assert(!admin_);

		admin_ = this;

		device_ = new device();
		device_context_ = new device_context(_hwnd);
		keyboard_ = new keyboard();
		mouse_ = new mouse();
		xinput_[0] = new xinput(0);
		xinput_[1] = new xinput(1);
		xinput_[2] = new xinput(2);
		xinput_[3] = new xinput(3);
	}

	administrator::~administrator()
	{
		delete device_;
		delete device_context_;
		delete keyboard_;
		delete mouse_;
		delete xinput_[0];
		delete xinput_[1];
		delete xinput_[2];
		delete xinput_[3];

		admin_ = nullptr;
	}
}