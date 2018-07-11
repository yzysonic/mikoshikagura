#include "SceneTitle.h"
#include "SceneClear.h"
#include "FadeScreen.h"
#include "Light.h"
#include "SceneGlobal.h"

void SceneClear::Init(void)
{
	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(true);

	Light::Init();

	title = new Object;
	title->AddComponent<Text>()->LoadFont("おつとめフォント");
	title->GetComponent<Text>()->SetSize(60, 0);
	title->GetComponent<Text>()->area = { 0,0,SystemParameters::ResolutionX,SystemParameters::ResolutionY };
	title->GetComponent<Text>()->SetSize(128, 0);
	title->GetComponent<Text>()->SetColor(Color::black);
	title->GetComponent<Text>()->SetFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	title->GetComponent<Text>()->SetText("ステージクリア！");

	dummy.position = Vector3(10.f, 70.f, 0.f);

	camera = new MainCamera;
	camera->render_target = RenderTarget::Get("rt_main");
	camera->SetTarget(&dummy);
	camera->setBackColor(Color(250, 250, 250, 255));
	RenderSpace::Get("default")->SetCamera(0, camera);

	SceneEnd = false;
	FadeScreen::FadeIn(Color::white, 1.0f);
}

void SceneClear::Update(void)
{
	if (!SceneEnd && GetKeyboardTrigger(DIK_RETURN))
	{
		FadeScreen::FadeOut(Color::black, 1.0f);
		SceneEnd = true;
	}
	if (SceneEnd && FadeScreen::Finished())
	{
		GameManager::GetInstance()->SetScene(new SceneTitle);
	}
}

void SceneClear::Uninit(void)
{
}
