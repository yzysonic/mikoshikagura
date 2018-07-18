#pragma once
#include "Core/Core.h"

class Snapper : public Object
{
public:
	Snapper();
	bool snapping;
	void OnCollisionEnter(Object *object) override;
	void OnCollisionExit(Object *object) override;
};

class CameraSnap : public Script
{
public:
	Transform *target = nullptr;
	std::vector<Snapper*> snappers;

	void Update(void) override;

private:
	Transform focus;
};
