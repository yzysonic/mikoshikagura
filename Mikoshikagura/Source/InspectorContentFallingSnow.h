#pragma once
#include "Core/Core.h"
#include "Inspector.h"
#include "FallingSnow.h"

class InspectorContentFallingSnow : public InspectorContent
{
public:
	void Init(void) override;
	void GuiContent(void) override;

private:
	FallingSnow * snow;
};