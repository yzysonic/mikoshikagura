#pragma once
#include "Core/Core.h"
#include "Core/Text.h"
#include "MainCamera.h"

class SceneClear : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	bool			SceneEnd;
	Object			*title;
	MainCamera		*camera;
	Transform		dummy;
};