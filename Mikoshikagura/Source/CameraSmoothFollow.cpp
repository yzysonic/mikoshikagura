#include "CameraSmoothFollow.h"
#include "CameraSphericalCoordinate.h"

void CameraSmoothFollow::Init(void)
{
	camera = dynamic_cast<Camera*>(object);

	if (!target)
	{
		SetActive(false);
	}
}

void CameraSmoothFollow::Update(void)
{
	camera->at = Vector3::Lerp(camera->at, target->position+Vector3(0.0f, offset_y, 0.0f), speed*Time::DeltaTime());
}