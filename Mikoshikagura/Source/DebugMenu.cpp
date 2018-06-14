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
	if (ImGui::BeginPopupModal("DebugMenu", NULL, ImGuiWindowFlags_NoResize))
	{
		// ���X�g�\��
		GuiContent();

		// �N���X�{�^��
		ImGui::Spacing();
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

void DebugMenu::GuiContent(void)
{
	auto gameManager = GameManager::GetInstance();

	// �V�[���u���b�N
	if (gameManager->GetScene() != nullptr)
	{
		ImGui::Text("Scene");
		OnPress(ImGui::Button("TakeSnapshot", buttonSize), [&] {
			auto fileName = gameManager->Var<std::string>("SnapshotName").c_str();
			DebugManager::TakeSnapshot(fileName, 0.3f);
		});
		OnPress(ImGui::Button("Reload", buttonSize), [&] {
			gameManager->ReloadScene();
		});
		OnPress(ImGui::Button("ObjectExplorer", buttonSize), [&] {
			DebugManager::OpenObjectExplorer();
		});
		if (DebugManager::GetFreeCamera())
		{
			OnPress(ImGui::Button("DisableFreeCamera", buttonSize), [&] {
				DebugManager::DisableFreeCamera();
			});
		}
		else
		{
			OnPress(ImGui::Button("EnableFreeCamera", buttonSize), [&] {
				DebugManager::EnableFreeCamera();
			});
		}
		
	}

	// �V�X�e���u���b�N
	ImGui::Text("System");
	OnPress(ImGui::Button("SceneExplorer", buttonSize), [&]
	{
		DebugManager::DisableFreeCamera();
		gameManager->SetScene(new SceneExplorer);
	});
	OnPress(ImGui::Button("ExitGame", buttonSize), [&]
	{
		Game::Stop();
	});

}

void DebugMenu::OnPress(bool trigger, std::function<void(void)> callBack)
{
	if (!trigger)
		return;

	callBack();
	SetActive(false);
}