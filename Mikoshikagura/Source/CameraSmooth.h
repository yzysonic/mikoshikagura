#pragma once
#include "Core/Core.h"

#define SmoothOffsetY (10.0f)

class CameraSmooth : public Script
{
public:
	Transform* target = nullptr;
	float speed;
	float offset_y = SmoothOffsetY;

	CameraSmooth(void) {};
	CameraSmooth(Object* target) : target(&target->transform) {};

	void Init(void) override;
	void Update(void) override;

private:
	Camera* camera;
};

