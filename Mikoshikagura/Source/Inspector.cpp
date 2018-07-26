#include "Inspector.h"
#include "DebugManager.h"

Inspector::Inspector(void) : extensionContent(nullptr)
{
	active_on_load = false;
	flags = 0;
	flags |= ImGuiWindowFlags_NoSavedSettings;
}

void Inspector::Init(void)
{
	active = true;
}

void Inspector::Update(void)
{
	ImGui::SetNextWindowPos(ImVec2((float)SystemParameters::ResolutionX-(Width+10.0f), 10.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(Width, 0.0f), ImGuiCond_Always);
	ImGui::Begin("Inspector", &active, flags);
	GuiContent();
	if (extensionContent)
		extensionContent->GuiContent();
	ImGui::End();

	if (!active)
	{
		SetActive(false);
	}
}

void Inspector::GuiContent(void)
{
	auto rotation = object->transform.getRotation();

	// Name
	auto active = object->GetActive();
	if (ImGui::Checkbox(object->name.c_str(), &active))
	{
		object->SetActive(active);
	}

	// Transform
	if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("position", (float*)&object->transform.position);
		if (ImGui::DragFloat3("rotation", (float*)&rotation)) { object->transform.setRotation(rotation); }
		ImGui::DragFloat3("scale", (float*)&object->transform.scale);

		ImGui::Spacing();
		ImGui::TreePop();
	}

	// Rigidbody
	if (rigidbody)
	{
		if (ImGui::TreeNodeEx("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto active = rigidbody->GetActive();
			if (ImGui::Checkbox("Active", &active)) { rigidbody->SetActive(active); }
			ImGui::Checkbox("Gravity", &rigidbody->useGravity);
			ImGui::DragFloat3("position", (float*)&rigidbody->position);
			ImGui::DragFloat3("rotation", (float*)&rigidbody->rotation);
			ImGui::DragFloat3("velocity", (float*)&rigidbody->velocity);
			ImGui::DragFloat3("acceleration", (float*)&rigidbody->acceleration);

			ImGui::Spacing();
			ImGui::TreePop();
		}

	}

	// BoxCollider2D
	if (boxcollider2d)
	{
		if (ImGui::TreeNodeEx("BoxCollider2D", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto active = boxcollider2d->GetActive();
			if (ImGui::Checkbox("Active", &active)) { boxcollider2d->SetActive(active); }
			ImGui::DragFloat2("size", (float*)&boxcollider2d->size);
			ImGui::DragFloat2("offset", (float*)&boxcollider2d->offset);

			ImGui::Spacing();
			ImGui::TreePop();
		}
	}

	// Scripts
	if (!script_list.empty())
	{
		if (ImGui::TreeNodeEx("Script", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto component : script_list)
			{
				if (component->GetName() == name)
				{
					continue;
				}

				auto active = component->GetActive();
				if (ImGui::Checkbox(component->GetName(), &active)) { component->SetActive(active); }
			}
			ImGui::Spacing();
			ImGui::TreePop();
		}
	}
}

void Inspector::SetObject(Object * object)
{
	this->object = object;
	extensionContent = nullptr;

	if (object == nullptr)
		return;

	auto inspectorExtention = object->GetComponent<InspectorExtension>();
	if (inspectorExtention)
	{
		extensionContent = inspectorExtention->extensionContent.get();
	}

	object->GetScripts().swap(script_list);
	rigidbody = object->GetComponent<Rigidbody>();
	boxcollider2d = object->GetComponent<BoxCollider2D>();
}

Object * Inspector::GetObject(void)
{
	return object;
}

InspectorExtension::InspectorExtension(InspectorContent * extensionContent) : extensionContent(extensionContent)
{
}

InspectorExtension::~InspectorExtension(void)
{
	if (DebugManager::GetInstance() && DebugManager::GetInspector()->GetObject() == object)
		DebugManager::GetInspector()->SetActive(false);
}

void InspectorExtension::Init(void)
{
	extensionContent->object = object;
	extensionContent->Init();
}