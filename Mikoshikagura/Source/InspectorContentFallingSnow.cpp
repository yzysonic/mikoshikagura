#include "InspectorContentFallingSnow.h"

void InspectorContentFallingSnow::Init(void)
{
	snow = dynamic_cast<FallingSnow*>(object);
}

void InspectorContentFallingSnow::GuiContent(void)
{
	if (ImGui::TreeNodeEx("SnowSettings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto &behavior = snow->behavior;
		ImGui::DragFloat("falling speed", &snow->behavior.falling_speed, 0.5f, 0.0, 1000.0f);
		ImGui::DragFloat("noise scale", &snow->behavior.noise_scale, 0.1f, 0.0f, 100.0f);
		ImGui::DragFloat("noise frequency", &snow->behavior.noise_frequency, 0.001f, 0.0f, 10.0f);
		ImGui::DragInt("noise octavers", &snow->behavior.noise_octavers, 1.0f, 0, 6);

		ImGui::Spacing();
		ImGui::TreePop();
	}
}


