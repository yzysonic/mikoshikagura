#pragma once
#include "Core/Core.h"
#include "Player.h"
#include "SeasonManager.h"
#include "DebugMenu.h"

class SeasonTestObject : public SeasonObject
{
public:
	SeasonTestObject(void);
	void SetSummer(void) override;
	void SetWinter(void) override;
	void Update(void) override;
	void OnCollisionStay(Object* object) override;

private:
	StaticModel * model;
	BoxCollider2D * collider;
	bool collide;
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
	Camera * camera;
	DebugMenu * debug;
	FrameTimer timer;
};