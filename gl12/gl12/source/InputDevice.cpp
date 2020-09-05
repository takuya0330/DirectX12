#include "InputDevice.h"

namespace gl::detail
{
	bool InputDevice::State(bool _IsPress, Trigger _Trigger)
	{
		mPreviousState = mCurrentState;
		if (_IsPress) mCurrentState++;
		else mCurrentState = 0;

		if (_Trigger == Trigger::ePress) return mPreviousState == 0 && mCurrentState > 0;
		else if (_Trigger == Trigger::eRelease) return mPreviousState > 0 && mCurrentState == 0;
		else return mCurrentState > 0;

		return false;
	}
}