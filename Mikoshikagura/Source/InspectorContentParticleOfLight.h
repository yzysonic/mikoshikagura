#pragma once
#include "Core/Core.h"
#include "Inspector.h"
#include "ParticleOfLight.h"

class InspectorContentParticleOfLight : public InspectorContent
{
public:
	void Init(void) override;
	void GuiContent(void) override;

private:
	ParticleOfLight * light;
};