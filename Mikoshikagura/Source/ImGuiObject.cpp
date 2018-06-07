#include "ImGuiObject.h"

void ImGuiObject::Init(void)
{
	object->GetComponents().swap(script_list);
	script_count = object->GetScripts().size();
	flags = 0;
	flags |= ImGuiWindowFlags_NoSavedSettings;

}

void ImGuiObject::Update(void)
{
	ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300.0f, 0.0f), ImGuiCond_Once);
	ImGui::Begin(object->name.c_str(), NULL, flags);
	GuiContent();
	ImGui::End();
}

void ImGuiObject::GuiContent(void)
{
	auto rotation = object->transform.getRotation();

	// Transform
	if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("position", (float*)&object->transform.position);
		if (ImGui::DragFloat3("rotation", (float*)&rotation)) { object->transform.setRotation(rotation); }
		ImGui::DragFloat3("scale", (float*)&object->transform.scale);
		
		ImGui::Spacing();
		ImGui::TreePop();
	}

	// Scripts
	if (script_count > 0)
	{
		if (ImGui::TreeNodeEx("Script", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto component : script_list)
			{
				if (component->GetType() != ComponentType::Script)
					continue;

				auto active = component->GetActive();
				if (ImGui::Checkbox(component->GetName(), &active)) { component->SetActive(active); }
			}
			ImGui::Spacing();
			ImGui::TreePop();
		}
	}
	

}
