#pragma once
#include "Core/Core.h"
#include "Core/Text.h"
#include "MainCamera.h"
#include "Background.h"
#include "SeasonManager.h"
#include "SeasonSoundPlayer.h"
#include "SunLight.h"

class SceneTitle : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	bool			next_scene;
	Object			*title;
	SeasonManager	*seasonmanager;
	Background		*background;
	MainCamera		*camera;
	Transform		dummy;
	SeasonSoundPlayer *environment_player;
	SunLight		*sun_light[20];

};