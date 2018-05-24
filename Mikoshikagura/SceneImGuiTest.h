#pragma once
#include "Core\Core.h"

class SceneImGuiTest : public Scene
{
#ifdef IMGUI
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	Camera* camera;
	ImVec4 clear_color;
	bool show_demo_window;
	bool show_another_window;
#else
public:
	void Update(void) override {};

#endif
};