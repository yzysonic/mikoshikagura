#include "CameraSnap.h"
#include "CameraSmooth.h"

void CameraSnap::Init(void)
{
	camera = dynamic_cast<Camera*>(object);
}

void CameraSnap::Update(void)
{
	float base_length = SnapDistance;
	this->focus = new Transform();
	auto smooth = this->object->GetComponent<CameraSmooth>();
	for (auto snapper = this->snappers.begin(); snapper != this->snappers.end(); ++snapper)
	{
		float distance = (this->target->position.toVector2() - (*snapper)->position.toVector2()).length();

		if (distance < base_length)
		{
			base_length = distance;
			this->focus->position = Vector3::Lerp(this->target->position, (*snapper)->position, 0.5f);
			smooth->target = this->focus;
		}
	}

	if (base_length == SnapDistance)
	{
		smooth->target = this->target;
	}
}
