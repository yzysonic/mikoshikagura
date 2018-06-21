#include "InspectorContentParticleOfLight.h"

void InspectorContentParticleOfLight::Init(void)
{
	light = dynamic_cast<ParticleOfLight*>(object);
}

void InspectorContentParticleOfLight::GuiContent(void)
{
	if (ImGui::TreeNodeEx("LightSettings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto &behavior = light->behavior;
		ImGui::DragFloat("speed", &light->behavior.rise_speed, 0.5f, 0.0, 1000.0f);
		ImGui::DragFloat("noise scale", &light->behavior.noise_scale, 0.1f, 0.0f, 100.0f);
		ImGui::DragFloat("noise frequency", &light->behavior.noise_frequency, 0.001f, 0.0f, 10.0f);
		ImGui::DragInt("noise octavers", &light->behavior.noise_octavers, 1.0f, 0, 6);

		ImGui::Spacing();
		ImGui::TreePop();
	}
}


