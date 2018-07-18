#pragma once

#include "Core/Core.h"
#include "Player.h"
#include "MainCamera.h"
#include "SeasonManager.h"
#include "SeasonModel.h"
#include "DebugMenu.h"
#include "Item.h"

class SeasonTestObject : public Object
{
public:
	SeasonTestObject(void);
	void Update(void) override;
	void OnCollisionEnter(Object* other) override;
	void OnCollisionStay(Object* object) override;
	void OnCollisionExit(Object* other) override;

private:
	SeasonModel * model;
	BoxCollider2D * collider;
	bool collide;

	void SwitchModel(void);
}; 

class SeasonBgmPlayerTest : public Object, public ISeason
{
public:
	SeasonBgmPlayerTest(void);
	void SetSummer(void) override;
	void SetWinter(void) override;
	void Play(void);

private:
	SoundPlayer * player;
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
	Item * item;
	SeasonBgmPlayerTest * bgm_player;
	Object * snow;
	Object * test_model;
};