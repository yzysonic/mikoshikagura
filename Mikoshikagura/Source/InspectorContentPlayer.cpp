#include "InspectorContentPlayer.h"

void InspectorContentPlayer::Init(void)
{
	player = dynamic_cast<Player*>(object);
}

void InspectorContentPlayer::GuiContent(void)
{
	if (ImGui::TreeNodeEx("PlayerInformation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("State: %s", player->current_state->ToString());
		ImGui::Text("Grounded: %s", player->is_grounded ? "true" : "false");
		for (auto ground_collider : player->ground_colliders)
		{
			ImGui::Text("Ground Collider: %x", ground_collider);
		}
		

		ImGui::Spacing();
		ImGui::TreePop();
	}
}
