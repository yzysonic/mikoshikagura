#pragma once
#include "Core/Core.h"

class ImGuiObject : public Script
{
public:
	virtual void Init(void) override;
	virtual void Update(void) override;

protected:
	virtual void GuiContent(void);

private:
	std::vector<Component*> script_list;
	ImGuiWindowFlags flags;
};