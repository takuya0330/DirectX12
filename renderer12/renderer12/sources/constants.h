#pragma once
#include "math.h"

namespace constants
{
	constexpr int2 kResolution = int2(1600, 900);
	constexpr float kAspectRatio = static_cast<float>(kResolution.x) / static_cast<float>(kResolution.y);
}