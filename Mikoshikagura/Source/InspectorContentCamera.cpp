#include "InspectorContentCamera.h"
#include "CameraSphericalCoordinate.h"
#include "CameraSmoothFallow.h"

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
		auto spherical = camera->GetComponent<CameraSphericalCoordinate>();
		if (spherical)
		{
			ImGui::Text("SphericalCoordinate:");
			ImGui::DragFloat("phi", &spherical->phi, 0.001f);
			ImGui::DragFloat("theta", &spherical->theta, 0.001f);
			ImGui::DragFloat("distance", &spherical->distance);
		}

		ImGui::Spacing();
		auto smooth = camera->GetComponent<CameraSmoothFallow>();
		if (smooth)
		{
			ImGui::Text("SmoothFallow:");
			ImGui::DragFloat("speed", &smooth->speed);
			ImGui::DragFloat("offset_y", &smooth->offset_y);
		}
		ImGui::TreePop();
	}

}
