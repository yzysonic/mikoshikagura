#include "MainCamera.h"
#include "CameraPlay.h"
#include "CameraSmooth.h"

MainCamera::MainCamera(void)
{
	this->far_z = 1000.0f;
	this->transform.position = Vector3(0.0f, 0.0f, -150.0f);
	this->fov = Deg2Rad(60.0f);
	this->AddComponent<CameraPlay>();
	this->AddComponent<CameraSmooth>();
}

void MainCamera::SetTarget(Transform * target)
{
	auto smooth = this->GetComponent<CameraSmooth>();
	this->target = target;
	smooth->target = target;
	smooth->SetActive(true);
}

void MainCamera::AddSnapper(Transform* target)
{
	this->snappers.push_back(target);
}

void MainCamera::RemoveSnapper(Transform* target)
{
	auto snapper = std::find(this->snappers.begin(), this->snappers.end(), target);
	if (snapper != this->snappers.end())
	{
		this->snappers.erase(snapper);
	}
}

void MainCamera::SearchSnapper(void)
{
	float base_length = 50.0f;
	this->focus = new Transform();
	auto smooth = this->GetComponent<CameraSmooth>();
	for (auto snapper = this->snappers.begin(); snapper != this->snappers.end(); ++snapper)
	{
		float distance = (this->target->position.toVector2() - (*snapper)->position.toVector2()).length();
	
		if (distance < base_length)
		{
			base_length = distance;
			this->focus->position = (this->target->position + (*snapper)->position) * 0.5f;
			smooth->target = this->focus;
		}
	}

	if(base_length == 50.0f)
	{
		smooth->target = this->target;
	}
}
