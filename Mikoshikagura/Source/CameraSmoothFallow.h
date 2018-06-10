#pragma once
#include "Core/Core.h"

class CameraSmoothFallow : public Script
{
public:
	Transform* target	= nullptr;
	float speed			= 5.0f;
	float offset_y		= 10.0f;

	CameraSmoothFallow(void) {};
	CameraSmoothFallow(Object* target) : target(&target->transform) {};

	void Init(void) override;
	void Update(void) override;

private:
	Camera* camera;

	inline void MoveCamera(void);
};

