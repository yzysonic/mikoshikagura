#pragma once
#include "Core/Core.h"
#include "Inspector.h"
#include "Player.h"

class InspectorContentPlayer : public InspectorContent
{
public:
	void Init(void) override;
	void GuiContent(void) override;

private:
	Player * player;
	std::string state;
};