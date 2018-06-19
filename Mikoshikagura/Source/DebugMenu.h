#pragma once

#include "Core\Core.h"
#include "Imgui/ImGuiImpl.h"

class DebugMenu : public Script
{
public:
	static constexpr float MenuWidth = 300.0f;

	DebugMenu(void);
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
	void GuiContent(void);

private:
	ImVec2 buttonSize;
	void OnPress(bool trigger, std::function<void(void)> callBack = [] {});
};