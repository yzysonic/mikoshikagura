#include "SceneTitle.h"
#include "Scene_Stage1.h"
#include "FadeScreen.h"
#include "Light.h"

void SceneTitle::Init(void)
{
	FadeScreen::FadeIn(Color::black, 1.0f);
	Light::Init();

	Texture::Load("mikoshikagura_rogo2");

	RenderSpace::Get("default")->GetCamera(0)->setBackColor(Color::black);

	title = new Object;
	title->AddComponent<RectPolygon2D>("mikoshikagura_rogo2")->SetSize(960, 240);
	title->transform.position = Vector3(0, 100, 0);
	title->AddComponent<Text>()->LoadFont("おつとめフォント");
	title->GetComponent<Text>()->area = { 475,400,1000,500 };

	SceneEnd = false;
}

void SceneTitle::Update(void)
{
	if (!SceneEnd && GetKeyboardTrigger(DIK_RETURN))
	{
		FadeScreen::FadeOut(Color::white, 1.0f);
		SceneEnd = true;
	}
	if (!SceneEnd && FadeScreen::Finished())
	{
		title->GetComponent<Text>()->SetText("エンターキーではじめる");
	}
	if (SceneEnd && FadeScreen::Finished())
	{
		GameManager::GetInstance()->SetScene(new Scene_Stage1);
	}
}

void SceneTitle::Uninit(void)
{
	Texture::Release("mikoshikagura_rogo2");
}
