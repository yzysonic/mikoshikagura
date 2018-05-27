#pragma once
#include "Core/Core.h"
#include "ImGuiObject.h"

class ImGuiCamera : public ImGuiObject
{
public :
	void Init(void) override;
	void GuiContent(void) override;

private:
	Camera * camera;
	Colorf back_color;
};