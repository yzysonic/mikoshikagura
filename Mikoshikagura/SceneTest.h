#pragma once
#include "Core/Core.h"
#include "MainCamera.h"
#include "Player.h"

class SceneTest : public Scene
{
public:
	void Init(void) override;
	void Update(void)override;
	void Uninit(void) override;

private:
	Object *test;
	MainCamera *camera;
	Player *player;
};