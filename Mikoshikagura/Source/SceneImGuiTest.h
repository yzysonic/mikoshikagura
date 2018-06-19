#pragma once
#include "Core\Core.h"
#include "Imgui/ImGuiImpl.h"

class SceneImGuiTest : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	Camera* camera;
	ImVec4 clear_color;
	bool show_demo_window;
	bool show_another_window;
};