#include "SceneStackSampleGame.h"
#include "SceneStackSampleOp.h"
#include "FadeScreen.h"
#include "SceneGlobal.h"

void SceneStackSampleGame::Init(void)
{
	Texture::Load("Snapshot_SceneTest");

	image = new Object;
	image->AddComponent<RectPolygon2D>("Snapshot_SceneTest");

	FadeScreen::FadeIn(Color::black, 1.0f);
	state = State::Capture;
}

void SceneStackSampleGame::Update(void)
{
	static int i = 0;
	switch (state)
	{
	case State::Capture:
		GameManager::GetInstance()->PushScene(new SceneStackSampleOp);
		state = State::Run;
		break;

	case State::Run:
		break;
	}
}

void SceneStackSampleGame::Uninit(void)
{
	Texture::Release("Snapshot_SceneTest");

	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(false);
}
