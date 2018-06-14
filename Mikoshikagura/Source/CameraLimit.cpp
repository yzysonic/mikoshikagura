#include "CameraLimit.h"

void CameraLimit::Init(void)
{
	camera = dynamic_cast<Camera*>(object);
	limit = 70.0f;
}

void CameraLimit::Update(void)
{
	if (camera->at.x < limit)
	{
		camera->at.x = limit;
	}
}