#pragma once
#include "Core/Core.h"

#define LimitDistance (80.0f)

class CameraLimit : public Script
{
public:
	Transform *target = nullptr;

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	Camera* camera;
	Transform* focus = nullptr;
};