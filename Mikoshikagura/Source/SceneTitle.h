#pragma once
#include "Core/Core.h"
#include "Core/Text.h"
#include "MainCamera.h"
#include "Player.h"
#include "Background.h"
#include "SeasonManager.h"

class SceneTitle : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	bool SceneEnd;
	Object *title;
	SeasonManager	*seasonmanager;
	Background		*background;
	MainCamera		*camera;
	Transform		dummy;
	Vector3			dummy_pos;
	float			radian;
};