#pragma once
#include "Core/Core.h"
#include "Core/Text.h"

class SceneTitle : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	bool SceneEnd;
	Object *title;
	
};