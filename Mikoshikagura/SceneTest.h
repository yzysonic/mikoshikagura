#pragma once
#include "Core/Core.h"


class SceneTest : public Scene
{
public:
	void Init(void) override;
	void Update(void)override;
	void Uninit(void) override;

private:
	Object *test;
};