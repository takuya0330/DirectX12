#pragma once
#include "mathematics.h"

namespace gl
{
	constexpr uint2 kWindowSize = uint2(1280, 720);

	constexpr uint2 kHalfWindowSize = kWindowSize / 2;

	constexpr float kAspectRatio = static_cast<float>(kWindowSize.x) / static_cast<float>(kWindowSize.y);

	constexpr uint kFrameBufferNum = 3;
}