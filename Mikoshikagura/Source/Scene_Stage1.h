#pragma once
#include "Core/Core.h"
#include "MainCamera.h"
#include "Player.h"
#include "MapManager.h"
#include "Background.h"
#include "SeasonManager.h"
#include "GoalObject.h"
#include "SceneTitle.h"
#include "FallingSnow.h"
#include "SunLight.h"
#include "FieldBgmPlayer.h"

#include "ParticleOfLight.h"

class Scene_Stage1 : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	MainCamera		*camera;
	SeasonManager	*seasonmanager;
	Background		*background;
	FallingSnow		*falling_snow;
	ParticleOfLight	*light_particle;
	MapManager		*mapdata;
	Hukidashi		*hukidashi;
	Player			*player;
	FieldBgmPlayer	*field_bgm_player;
	GoalObject<SceneTitle> *goal;
	SunLight		*sun_light[20];

	//�e�X�g�p��
	Object *wall;
};