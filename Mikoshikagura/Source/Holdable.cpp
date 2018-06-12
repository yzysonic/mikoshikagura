#include "Holdable.h"

Holdable::Holdable(void)
{
	active_on_load = false;
	owner = nullptr;
	offset_y = 0.0f;
}

void Holdable::Update(void)
{
	object->transform.position = owner->position + Vector3(0.0f, offset_y, 0.0f);
}

void Holdable::Uninit(void)
{
	auto rigidbody = object->GetComponent<Rigidbody>();
	rigidbody->SetActive(true);
	rigidbody->velocity = owner->object->GetComponent<Rigidbody>()->velocity;
}

void Holdable::SetOwner(Transform * owner)
{
	if (owner)
	{
		this->owner = owner;
		SetActive(true);
	}
	else
	{
		SetActive(false);
	}
}
