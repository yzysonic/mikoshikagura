#pragma once
#include "Core/Core.h"
#include "MainCamera.h"
#include "Player.h"
#include "MapManager.h"
#include "Background.h"
#include "SeasonManager.h"
#include "GoalObject.h"
#include "FallingSnow.h"
#include "SunLight.h"
#include "ParticleOfLight.h"
#include "SeasonSoundPlayer.h"

class SceneTest : public Scene
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
	GoalObject<SceneTest> *goal;
	SunLight		*sun_light[20];
	Snapper			*snapper;
	SeasonSoundPlayer	*field_bgm_player;
	SeasonSoundPlayer	*environment_sound_player;

	//�e�X�g�p��
	Object *wall;
};