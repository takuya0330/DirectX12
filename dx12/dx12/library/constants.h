#pragma once
#include "math.h"

namespace snd::constants
{
	constexpr uint2 kResolution = uint2(1600, 900);

	constexpr uint2 kHalfResolution = kResolution / 2;

	constexpr float kAspectRatio = static_cast<float>(kResolution.x) / static_cast<float>(kResolution.y);

	constexpr uint kBackBufferCount = 2;
}