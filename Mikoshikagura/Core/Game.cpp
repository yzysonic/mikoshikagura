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

bool Game::run_game;

void Game::Init(void)
{
	Time::Init();
	ObjectManager::Create();
	GameManager::Create();
	Renderer::Create();
	Physics::Create();
	InitRandom();
	Texture::Init();
	run_game = true;
}

void Game::Run(void)
{
	while (run_game)
	{
		Window::CheckMesg();
		UpdateInput();
		Physics::Update();
		GameManager::Update();
		ObjectManager::Update();
		Renderer::DrawFrame();
		Time::FramerateControl();
	}
}

void Game::Uninit(void)
{
	GameManager::Destroy();
	ObjectManager::Destroy();
	Texture::Uninit();
	Physics::Destroy();
	Renderer::Destroy();
	Time::Uninit();
}

bool Game::End(void)
{
	return true;
}

void Game::Stop(void)
{
	run_game = false;
}
