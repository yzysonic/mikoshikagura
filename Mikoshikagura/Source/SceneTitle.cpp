#include "SceneTitle.h"
#include "Scene_Stage1.h"
#include "FadeScreen.h"
#include "Light.h"

void SceneTitle::Init(void)
{
	FadeScreen::FadeIn(Color::black, 1.0f);
	Light::Init();

	Texture::Load("mikoshikagura_rogo");

	Renderer::GetInstance()->getCamera()->setBackColor(Color::black);

	title = new Object;
	title->AddComponent<RectPolygon2D>("mikoshikagura_rogo")->SetSize(960, 222);
	title->transform.position = Vector3(0, 100, 0);
	title->AddComponent<Text>()->LoadFont("���Ƃ߃t�H���g");
	title->GetComponent<Text>()->area = { 500,400,1000,500 };

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
		title->GetComponent<Text>()->SetText("�G���^�[�L�[�ł͂��߂�");
	}
	if (SceneEnd && FadeScreen::Finished())
	{
		GameManager::GetInstance()->SetScene(new Scene_Stage1);
	}
}

void SceneTitle::Uninit(void)
{
}
