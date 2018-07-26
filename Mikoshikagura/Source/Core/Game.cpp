#include "Game.h"
#include "Window.h"
#include "Direct3D.h"
#include "Input.h"
#include "Time.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "Physics.h"
#include "GameManager.h"
#include "Random.h"
#include "Texture.h"

#ifdef _DEBUG
#include "../Imgui/ImGuiImpl.h"
#endif

bool Game::run_game;
std::function<void()> Game::EndProcess = [] {Game::Stop(); };

void Game::Init(void)
{
	Time::Init();
	ObjectManager::Create();
	GameManager::Create();
	Renderer::Create();
	Physics::Create();
	InitRandom();
	run_game = true;
}

void Game::Run(void)
{
	while (run_game)
	{
		Window::CheckMesg();
		UpdateInput();

#ifdef IMGUI
		ImGuiImpl::NewFrame();
#endif

		Physics::Update();
		GameManager::Update();
		ObjectManager::Update();

#ifdef IMGUI
		ImGuiImpl::EndFrame();
#endif
		Renderer::DrawFrame();
		Time::FramerateControl();
	}
}

void Game::Uninit(void)
{
	GameManager::Destroy();
	ObjectManager::Destroy();
	Physics::Destroy();
	Renderer::Destroy();
	Time::Uninit();
}

bool Game::EndPrompt(void)
{
#ifdef _DEBUG
	Game::Stop();
	return true;
#endif
	if (MessageBox(Window::GetHWnd(), "�I�����܂����H", "�I���m�F", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		Game::EndProcess();
		return true;
	}

	return false;
}

void Game::Stop(void)
{
	if(Window::GetHWnd())
		Window::Destroy();
	else
		run_game = false;
}
