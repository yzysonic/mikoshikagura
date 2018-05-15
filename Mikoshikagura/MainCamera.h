#pragma once
#include "Core/Core.h"

class MainCamera :public Camera
{
public:
	MainCamera(void);
	void  SetTarget(Transform* target);
};