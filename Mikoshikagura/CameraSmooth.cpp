#include "CameraSmooth.h"

void CameraSmooth::Init(void)
{
	camera = dynamic_cast<Camera*>(object);
	speed = 5.0f;
}

void CameraSmooth::Update(void)
{
	if (target == nullptr)
	{
		SetActive(false);
		return;
	}

	camera->at = Vector3::Lerp(camera->at, target->position+Vector3(0.0f, offset_y, 0.0f), speed*Time::DeltaTime());
}