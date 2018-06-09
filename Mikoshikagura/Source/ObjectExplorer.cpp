#include "ObjectExplorer.h"
#include "DebugManager.h"

ObjectExplorer::ObjectExplorer(void)
{
	active_on_load = false;
}

void ObjectExplorer::Init(void)
{
	selected = -1;
	active = true;
}

void ObjectExplorer::Update(void)
{
	if (!DebugManager::GetInspector()->GetActive())
	{
		selected = -1;
	}

	auto objectList = GameManager::GetInstance()->GetScene()->GetObjectList();

	ImGui::SetNextWindowSize(ImVec2(Width, 0.0f), ImGuiCond_Once);
	ImGui::Begin("ObjectExplorer", &active);
	
	for (size_t n = 0; n < objectList.size(); n++)
	{
		auto obj = objectList[n];
		char name[128];
		sprintf(name, "%02d %s", n, obj->name.c_str());
		if (ImGui::Selectable(name, selected == n) && selected != n)
		{
			DebugManager::OpenInspector(obj);
			selected = n;
		}
	}

	ImGui::End();

	if (!active)
	{
		SetActive(false);
	}
}
