#include "MainCamera.h"

MainCamera::MainCamera(void)
{
	far_z	= 1000.0f;
	fov		= Deg2Rad(60.0f);

	coordinate	= AddComponent<CameraSphericalCoordinate>();
	smooth		= AddComponent<CameraSmoothFallow>();
	snap		= AddComponent<CameraSnap>();
	limit		= AddComponent<CameraLimit>();

	coordinate->theta		= 1.444f;
	coordinate->distance	= 65.0f;
}

void MainCamera::SetTarget(Transform * target)
{
	at = target->position;
	smooth->target = target;
	smooth->SetActive(true);

	snap->target = target;
	snap->SetActive(true);
}

void MainCamera::AddSnapper(Transform* target)
{
	snap->snappers.push_back(target);
}

void MainCamera::RemoveSnapper(Transform* target)
{
	auto snapper = std::find(snap->snappers.begin(), snap->snappers.end(), target);
	if (snapper != snap->snappers.end())
	{
		snap->snappers.erase(snapper);
	}
}