#pragma once
#include "Core/Core.h"
#include "Inspector.h"

class InspectorContentCamera : public InspectorContent
{
public :
	void Init(void) override;
	void GuiContent(void) override;

private:
	Camera * camera;
	Colorf back_color;
};