#include "InspectorContentCamera.h"

void InspectorContentCamera::Init(void)
{
	camera = dynamic_cast<Camera*>(object);
	back_color = camera->backColor;
}

void InspectorContentCamera::GuiContent(void)
{
	if (ImGui::TreeNodeEx("CameraSettings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("fov", &camera->fov, 0.01f, 0.0f, Deg2Rad(180.0f));
		ImGui::DragFloat("aspect", &camera->aspect, 0.01f, 0.0f, 3.0f);
		ImGui::DragFloat("near", &camera->near_z);
		ImGui::DragFloat("far", &camera->far_z);
		if (ImGui::ColorEdit3("BackColor", (float*)&back_color)) { camera->setBackColor(back_color); }

		ImGui::Spacing();
		ImGui::TreePop();
	}

}
