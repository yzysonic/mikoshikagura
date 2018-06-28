#include "CameraSnap.h"
#include "CameraSmoothFollow.h"


void CameraSnap::Update(void)
{
	auto smooth = this->object->GetComponent<CameraSmoothFollow>();
	
	bool snapped = false;
	for (auto snapper : snappers)
	{
		if (snapper->snapping)
		{
			this->focus.position = Vector3::Lerp(this->target->position, snapper->transform.position, 0.5f);
			smooth->target = &this->focus;
			snapped = true;
		}
	}

	if (!snapped)
	{
		smooth->target = this->target;
	}
}

Snapper::Snapper()
{
	name = "snapper";
	snapping = false;
}

void Snapper::OnCollisionEnter(Object * object)
{
	if (object->type == ObjectType::Player)
		snapping = true;
}

void Snapper::OnCollisionExit(Object * object)
{
	if (object->type == ObjectType::Player)
		snapping = false;
}
