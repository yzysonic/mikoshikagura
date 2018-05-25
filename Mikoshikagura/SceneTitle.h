#pragma once
#include "Core/Core.h"
#include "MainCamera.h"

class SceneTitle : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	MainCamera *camera;
	bool SceneEnd;
};