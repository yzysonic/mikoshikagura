#ifdef IMGUI

#include "DebugMenu.h"
#include "Core\Game.h"
#include "SceneExplorer.h"
#include "Snapshot.h"
#include "DebugManager.h"

DebugMenu::DebugMenu(void)
{
	active_on_load = false;
	buttonSize = ImVec2(-1.0f, 0.0f);
}

void DebugMenu::Init(void)
{
	ImGui::OpenPopup("DebugMenu");
}

void DebugMenu::Update(void)
{
	ImGui::SetNextWindowSize(ImVec2(MenuWidth, 0.0f));
	
	// ���j���[�̕\��
	if (ImGui::BeginPopupModal("DebugMenu", NULL, ImGuiWindowFlags_NoResize))
	{
		auto gameManager = GameManager::GetInstance();
		
		// �V�[���u���b�N
		if (gameManager->GetScene() != nullptr)
		{
			ImGui::Text("Scene");
			OnPress(ImGui::Button("TakeSnapshot", buttonSize), [&]{
				auto fileName = gameManager->Var<std::string>("SnapshotName").c_str();
				DebugManager::TakeSnapshot(fileName, 0.3f);
			});
			OnPress(ImGui::Button("Reload", buttonSize), [&] {
				gameManager->ReloadScene();
			});
		}

		// �V�X�e���u���b�N
		ImGui::Text("System");
		OnPress(ImGui::Button("SceneExplorer", buttonSize), [&]
		{
			gameManager->SetScene(new SceneExplorer);
		});
		OnPress(ImGui::Button("ExitGame", buttonSize), [&]
		{
			Game::Stop();
		});

		// �N���X
		ImGui::Separator();
		ImGui::Spacing();
		OnPress(ImGui::Button("Close", buttonSize));

		ImGui::EndPopup();
	}
}

void DebugMenu::Uninit(void)
{
	ImGui::CloseCurrentPopup();
}

void DebugMenu::OnPress(bool trigger, std::function<void(void)> callBack)
{
	if (!trigger)
		return;

	callBack();
	SetActive(false);
}


#endif