#pragma once
#include"Core/Core.h"
#include"Scene_Stage1.h"
#include"Sunlight.h"

class SceneNagaiTest : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:

	Scene_Stage1 stage1;
	SunLight *sunlight[100];

};

