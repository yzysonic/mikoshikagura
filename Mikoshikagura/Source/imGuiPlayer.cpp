#include "imGuiPlayer.h"




void imGuiPlayer::Init(void)
{
	ImGuiObject::Init();
	player = dynamic_cast<Player*>(object);

}

void imGuiPlayer::GuiContent(void)
{

	ImGuiObject::GuiContent();
	if (ImGui::TreeNodeEx("CameraSettings", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::Spacing();
		ImGui::TreePop();
	}

}
