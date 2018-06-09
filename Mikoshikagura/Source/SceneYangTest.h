#pragma once
#include "Core/Core.h"
#include "Player.h"
#include "MainCamera.h"
#include "SeasonManager.h"
#include "DebugMenu.h"

class SeasonTestObject : public SeasonObject
{
public:
	SeasonTestObject(void);
	void SetSummer(void) override;
	void SetWinter(void) override;
	void Update(void) override;
	void OnCollisionEnter(Object* other) override;
	void OnCollisionStay(Object* object) override;
	void OnCollisionExit(Object* other) override;

private:
	StaticModel * model;
	ModelData * summer_model;
	ModelData * winter_model;
	BoxCollider2D * collider;
	bool collide;

	void SwitchModel(void);
};

class SceneYangTest : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	SeasonTestObject * test[10];
	Player * player;
	MainCamera * camera;
	DebugMenu * debug;
	FrameTimer timer;
};