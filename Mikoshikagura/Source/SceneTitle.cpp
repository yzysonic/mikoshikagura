#include "SceneTitle.h"
#include "Scene_Stage1.h"
#include "FadeScreen.h"
#include "Light.h"
#include "SceneGlobal.h"

void SceneTitle::Init(void)
{
	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(true);
	Texture::Load("mikoshikagura_rogo2");
	Texture::Load("background_summer_layer0");
	Texture::Load("background_summer_layer1");
	Texture::Load("background_summer_layer2");
	Texture::Load("background_winter_layer0");
	Texture::Load("background_winter_layer1");
	Texture::Load("background_winter_layer2");

	Light::Init();
	SeasonManager::Create(SeasonType::Summer);

	title = new Object;
	title->AddComponent<RectPolygon2D>("mikoshikagura_rogo2")->SetSize(960, 240);
	title->transform.position = Vector3(0, 100, 0);
	title->AddComponent<Text>()->LoadFont("おつとめフォント");
	title->GetComponent<Text>()->SetSize(60,0);
	title->GetComponent<Text>()->area = { 0,450,SystemParameters::ResolutionX,550 };
	title->GetComponent<Text>()->SetFormat(DT_SINGLELINE|DT_CENTER|DT_VCENTER);

	dummy.position = Vector3(10.f, 70.f, 0.f);

	camera = new MainCamera;
	camera->render_target = RenderTarget::Get("rt_main");
	camera->SetTarget(&dummy);
	camera->setBackColor(Color(250, 250, 250, 255));
	RenderSpace::Get("default")->SetCamera(0, camera);
	background = new Background;

	SceneEnd = false;
	FadeScreen::FadeIn(Color::black, 1.0f);
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
	Texture::Release("background_summer_layer0");
	Texture::Release("background_summer_layer1");
	Texture::Release("background_summer_layer2");
	Texture::Release("background_winter_layer0");
	Texture::Release("background_winter_layer1");
	Texture::Release("background_winter_layer2");
}
