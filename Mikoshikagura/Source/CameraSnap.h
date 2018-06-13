#pragma once
#include "Core/Core.h"

#define SnapDistance (50.0f)

class CameraSnap : public Script
{
public:
	Transform *target = nullptr;
	std::vector<Transform*> snappers;

	void Update(void) override;

private:
	Transform focus;
};
