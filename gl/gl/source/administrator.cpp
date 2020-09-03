#include "administrator.h"
#include "generate_device.h"
#include "rendering_device.h"
#include "keyboard.h"
#include "mouse.h"
#include "xinput.h"


namespace gl
{
	administrator::administrator(HWND _hwnd)
	{
		assert(!admin_);

		admin_ = this;

		generate_device_->initialize();
		rendering_device_->initialize(_hwnd);
		
	}

	administrator::~administrator()
	{
		generate_device_.release();
		rendering_device_.release();
		keyboard_.release();
		mouse_.release();
		xinput_.release();

		admin_ = nullptr;
	}
}