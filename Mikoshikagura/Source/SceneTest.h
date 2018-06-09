#pragma once
#include "Core/Core.h"
#include "MainCamera.h"
#include "Player.h"
#include "MapManager.h"
class SceneTest : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	Object *test;
	Object *testBG;
	Object *target1;
	Object *target2;
	MainCamera *camera;
	Player *player;

	MapManager *mapdata;

	std::vector<Object> mapobj;
};