#pragma once
#include "Core/Core.h"
#include "MainCamera.h"
#include "Player.h"
#include "MapManager.h"
#include "Background.h"
#include "SeasonManager.h"
#include "GoalObject.h"
#include "SceneTitle.h"

class Scene_Stage1 : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	MainCamera *camera;
	Player *player;
	MapManager *mapdata;
	Background *background;
	SeasonManager  *seasonmanager;

	Hukidashi *hukidashi;
	GoalObject<SceneTitle> *goal;

	//�e�X�g�p��
	Object *wall;
};