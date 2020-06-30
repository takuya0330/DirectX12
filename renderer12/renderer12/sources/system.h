#pragma once
#include "singleton.h"
#include "core.h"
#include "scene.h"

#include <memory>
#include <vector>

class System
{
public:
	System() {}
	~System() {}

	void Initialize(HWND hwnd);
	void UnInitialize();
	void Update();
	void Render();

private:
	D3D12Core d3d12core_;

private:
	friend SingletonHolder<System>;
};

using shSystem = SingletonHolder<System>;