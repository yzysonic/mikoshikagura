#include "SceneStackSample.h"
#include "SceneStackSampleGame.h"
#include "FadeScreen.h"
#include "SceneGlobal.h"

void SceneStackSample::Init(void)
{
	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(true);

	Texture::Load("scene_stack_sample_title");

	image = new Object;
	image->AddComponent<RectPolygon2D>("scene_stack_sample_title");

	next_scene = false;
}

void SceneStackSample::Update(void)
{
	if (next_scene)
	{
		if (FadeScreen::Finished())
		{
			GameManager::GetInstance()->SetScene(new SceneStackSampleGame);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			FadeScreen::FadeOut(Color::black, 1.0f);
			next_scene = true;
		}
	}
}

void SceneStackSample::Uninit(void)
{
	Texture::Release("scene_stack_sample_title");
}
