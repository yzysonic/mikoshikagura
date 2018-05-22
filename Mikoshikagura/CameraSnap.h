#pragma once
#include "Core/Core.h"

#define SnapDistance (50.0f)

class CameraSnap : public Script
{
public:
	Transform *target = nullptr;
	std::vector<Transform*> snappers;

	void Init(void) override;
	void Update(void) override;

private:
	Camera* camera;
	Transform* focus = nullptr;
};
