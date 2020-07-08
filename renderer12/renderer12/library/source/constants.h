#pragma once
#include "math.h"

namespace constants
{
	constexpr lib::int2 kResolution = lib::int2(1600, 900);

	constexpr lib::int2 kHalfResolution = kResolution / 2;

	constexpr float kAspectRatio = static_cast<float>(kResolution.x) / static_cast<float>(kResolution.y);
}