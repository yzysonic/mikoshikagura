#pragma once
#include "Core\Core.h"

class SceneStackSample : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	Object* image;
	bool next_scene;
};