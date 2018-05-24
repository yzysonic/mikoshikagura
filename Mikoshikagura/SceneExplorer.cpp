#include "SceneExplorer.h"
#include "SceneImGuiTest.h"
#include "SceneTest.h"
#include "DebugManager.h"

void SceneExplorer::Init(void)
{
	columns = 3;
	isAutoSnapshot = true;
	sceneList.reserve(30);

	// ImGuiウインドウの設定
	windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoNav;

	// スナップショット名
	GameManager::GetInstance()->Var<std::string>("SnapshotName") = "Snapshot_SceneExplorer";

	//シーンの登録
	AddScene<SceneImGuiTest>();
	AddScene<SceneTest>();
}

void SceneExplorer::Update(void)
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2((float)SystemParameters::ResolutionX, (float)SystemParameters::ResolutionY));
	ImGui::Begin("SceneExplorer", NULL, windowFlags);

	ImGui::SliderInt("Columns", &columns, 1, 5);
	
	ImGui::SameLine();
	ImGui::Checkbox("AutoSnapshot", &isAutoSnapshot);

	ImGui::Columns(columns, "", false);
	
	// シーンプレビュー表示
	for (int i = 0; i < (int)sceneList.size(); i++)
	{
		// タイトル
		ImGui::Text(sceneList[i]->name.c_str());

		// イメージボタン
		float buttonWidth = (float)(SystemParameters::ResolutionX / columns - 20);
		if (ImGui::ImageButton(sceneList[i]->texture->pDXTex, ImVec2(buttonWidth, buttonWidth*0.5625f)))
		{
			auto fileName = GameManager::GetInstance()->Var<std::string>("SnapshotName") = "Snapshot_" + sceneList[i]->name;
			if(isAutoSnapshot)
				DebugManager::TakeSnapshot(fileName.c_str(), 1.0f);
			sceneList[i]->SetScene();
			ImGui::End();
			return;
		}
		
		// 区切り
		if ((i+1) % columns == 0)
			ImGui::Separator();		
		ImGui::NextColumn();
	}

	
	ImGui::End();
}

void SceneExplorer::Uninit(void)
{
	for (auto& scene : sceneList)
	{
		scene->texture->Release();
	}
}
