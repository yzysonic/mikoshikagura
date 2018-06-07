#pragma once
#include "Core/Core.h"
#include "ImGuiObject.h"
#include "Player.h"

class imGuiPlayer : public ImGuiObject
{
public:
	void Init(void) override;
	void GuiContent(void) override;

	Player *player;
	Colorf back_color;
};

