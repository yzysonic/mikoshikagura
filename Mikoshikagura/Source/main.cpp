//=============================================================================
//
// �t�@�C�����Fmain.cpp
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#include "Core/System.h"
#include "Core/Window.h"
#include "Core/Game.h"
#include "Core/GameManager.h"
#include "SceneGlobal.h"

#ifndef _DEBUG
#include "SceneTitle.h"
#else
#include "SceneExplorer.h"
#endif

// ���������[�N�̎������o
#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#ifdef IMGUI
#include "Imgui/ImGuiImpl.h"
#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // ���������[�N�������o
#endif

	// �^�C�g��
	SystemParameters::windowTitle = "MIKOSHIKAGURA";

	// �V�X�e��������
	if (FAILED(System::Init()))
		return 1;

	// �V�X�e���J�[�\�����B��
	//ShowCursor(false);

#ifndef _DEBUG
	//Window::SetWindowMode(false);
#endif

#ifdef IMGUI
	// ImGui������
	ImGuiImpl::Create(Window::GetHWnd(), Direct3D::GetDevice());
#endif

	// �Q�[��������
	Game::Init();

	// �V�[��������
	GameManager::GetInstance()->SetGlobalScene(new SceneGlobal);
#ifndef _DEBUG
	GameManager::GetInstance()->SetScene(new SceneTitle);
#else
	GameManager::GetInstance()->SetScene(new SceneExplorer);
#endif

	// �Q�[�����[�v
	Game::Run();

	// �I��
	Game::Uninit();

#ifdef IMGUI
	// ImGui�̏I������
	ImGuiImpl::Destroy();
#endif

	System::Uninit();

	return 0;
}
