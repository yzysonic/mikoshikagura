#pragma once
#include "Core/Core.h"
#include "Player.h"

class SceneYangTest : public Scene
{
public:
	void Init(void) override;
	void Update(void) override {};
	void Uninit(void) override;

private:
	Object * test;
	Camera * camera;
};