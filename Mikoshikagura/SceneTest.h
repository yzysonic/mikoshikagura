#pragma once
#include "Core/Core.h"
#include "MainCamera.h"

class SceneTest : public Scene
{
public:
	void Init(void) override;
	void Update(void)override;
	void Uninit(void) override;

private:
	Object *test;
	Object *testBG;
	MainCamera *camera;
};