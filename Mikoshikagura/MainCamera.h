#pragma once
#include "Core/Core.h"

class MainCamera :public Camera
{
public:
	MainCamera(void);
	void SetTarget(Transform* target);
	void AddSnapper(Transform* target);
	void RemoveSnapper(Transform* target);
	void SearchSnapper(void);

private:
	Transform *target;
	Transform *focus;
	std::vector<Transform *> snappers;
};