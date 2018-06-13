#pragma once
#include "Core/Core.h"

class CameraLimit : public Script
{
public:
	void Init(void) override;
	void Update(void) override;

private:
	Camera* camera;
	float limit;
};