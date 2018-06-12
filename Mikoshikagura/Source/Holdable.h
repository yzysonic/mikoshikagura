#pragma once
#include "Core\Core.h"

class Holdable : public Script
{
public:
	Holdable(void);
	Transform* owner;
	float offset_y;

	void Update(void) override;
	void Uninit(void) override;
	void SetOwner(Transform* owner);
};