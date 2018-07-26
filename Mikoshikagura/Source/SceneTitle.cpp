#include "SceneTitle.h"
#include "Scene_Stage1.h"
#include "FadeScreen.h"
#include "Light.h"
#include "SceneGlobal.h"

void SceneTitle::Init(void)
{
	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(true);
	Texture::Load("mikoshikagura_rogo2");
	Sound::Load("voice_title");
	Sound::Load("game_start");
	Light::Init();
	SeasonManager::Create(SeasonType::Summer);

	environment_player = new SeasonSoundPlayer("environment");

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

	for (auto & light : sun_light)
		light = new SunLight;

	next_scene = false;

	FadeScreen::FadeIn(Color::black, 1.0f, [this] {
		Sound::Get("voice_title")->Play();
		title->GetComponent<Text>()->SetText("エンターキーではじめる");
	});
}

void SceneTitle::Update(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) && !next_scene)
	{
		Sound::Get("game_start")->Play();
		FadeScreen::FadeOut(Color::white, 1.0f, [this] {
			GameManager::GetInstance()->SetScene(new Scene_Stage1);
		});
		next_scene = true;
	}
	if (GetKeyboardTrigger(DIK_TAB))
	{
		SeasonManager::SwitchSeason();
	}
}

void SceneTitle::Uninit(void)
{
	Texture::Release("mikoshikagura_rogo2");
	Sound::Release("voice_title");
	Sound::Release("game_start");
}
