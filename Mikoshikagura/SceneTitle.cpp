#include "SceneTitle.h"
#include "SceneTest.h"
#include "FadeScreen.h"
#include "Light.h"

void SceneTitle::Init(void)
{
	FadeScreen::FadeIn(Color::black);
	Light::Init();

	camera = new MainCamera;
	Renderer::GetInstance()->setCamera(camera);

	SceneEnd = false;
}

void SceneTitle::Update(void)
{
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		FadeScreen::FadeOut(Color::black);
		SceneEnd = true;
	}
	if (SceneEnd && FadeScreen::Finished())
	{
		GameManager::GetInstance()->SetScene(new SceneTest);
	}
}

void SceneTitle::Uninit(void)
{
}
