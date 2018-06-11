#pragma once
#include "Core/Core.h"
#include "MainCamera.h"
#include "Player.h"
#include "Hukidashi.h"
#include "Sign.h"
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
	Sign *kanban;
	MainCamera *camera;
	Player *player;
	Hukidashi *hukidashi;

	MapManager *mapdata;

	std::vector<Object> mapobj;
};