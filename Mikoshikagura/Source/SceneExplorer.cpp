#include "SceneExplorer.h"
#include "DebugManager.h"
#include "FadeScreen.h"

#include "SceneImGuiTest.h"
#include "SceneTest.h"
#include "SceneTitle.h"
#include "SceneYangTest.h"
#include "SceneStackSample.h"
#include "Scene_Stage1.h"
#include "SceneClear.h"

#include "Scene_Stage2.h"
void SceneExplorer::Init(void)
{
	columns = 3;
	isAutoSnapshot = false;
	sceneList.reserve(30);

	// ImGui�E�C���h�E�̐ݒ�
	windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoNav;

	// �X�i�b�v�V���b�g��
	GameManager::GetInstance()->Var<std::string>("SnapshotName") = "Snapshot_SceneExplorer";

	//�V�[���̓o�^
	AddScene<SceneImGuiTest>();
	AddScene<SceneTest>();
	AddScene<SceneYangTest>();
	AddScene<SceneStackSample>();
	AddScene<SceneTitle>();

	AddScene<Scene_Stage1>();
	AddScene<SceneClear>();

	AddScene<Scene_Stage2>();

	FadeScreen::FadeIn(Color::black, 0.0f);
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
	
	// �V�[���v���r���[�\��
	for (int i = 0; i < (int)sceneList.size(); i++)
	{
		// �^�C�g��
		ImGui::Text(sceneList[i]->name.c_str());

		// �C���[�W�{�^��
		float buttonWidth = (float)(SystemParameters::ResolutionX / columns - 20);
		if (ImGui::ImageButton(sceneList[i]->texture->pDXTex, ImVec2(buttonWidth, buttonWidth*0.5625f)))
		{
			auto fileName = GameManager::GetInstance()->Var<std::string>("SnapshotName") = "Snapshot_" + sceneList[i]->name;
			if(isAutoSnapshot)
				DebugManager::TakeSnapshot(fileName.c_str(), 1.0f);
			ImGui::End();
			sceneList[i]->SetScene();
			return;
		}
		
		// ��؂�
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
		if (scene->texture != Texture::none)
			scene->texture->Release();
	}
	sceneList.clear();
}