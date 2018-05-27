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

private:
	StaticModel * model;
};

class SceneYangTest : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	SeasonTestObject * test;
	Player * player;
	Camera * camera;
	DebugMenu * debug;
};