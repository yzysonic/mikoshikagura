#pragma once
#include "Core\Core.h"

class DebugMenu : public Script
{
public:
	DebugMenu(void);
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	static constexpr float MenuWidth = 300.0f;
	ImVec2 buttonSize;
	void OnPress(bool trigger, std::function<void(void)> callBack = [] {});
};