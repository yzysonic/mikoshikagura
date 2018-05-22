#include "MainCamera.h"
#include "CameraPlay.h"
#include "CameraSmooth.h"
#include "CameraSnap.h"

MainCamera::MainCamera(void)
{
	this->far_z = 1000.0f;
	this->transform.position = Vector3(0.0f, 0.0f, -150.0f);
	this->fov = Deg2Rad(60.0f);
	this->AddComponent<CameraPlay>();
	this->AddComponent<CameraSmooth>();
	this->AddComponent<CameraSnap>();
}

void MainCamera::SetTarget(Transform * target)
{
	auto smooth = this->GetComponent<CameraSmooth>();
	smooth->target = target;
	smooth->SetActive(true);

	auto snap = this->GetComponent<CameraSnap>();
	snap->target = target;
	snap->SetActive(true);
}

void MainCamera::AddSnapper(Transform* target)
{
	auto snap = this->GetComponent<CameraSnap>();
	snap->snappers.push_back(target);
}

void MainCamera::RemoveSnapper(Transform* target)
{
	auto snap = this->GetComponent<CameraSnap>();
	auto snapper = std::find(snap->snappers.begin(), snap->snappers.end(), target);
	if (snapper != snap->snappers.end())
	{
		snap->snappers.erase(snapper);
	}
}