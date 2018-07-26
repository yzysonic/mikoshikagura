#include "SceneGlobal.h"
#include "SceneTest.h"
#include "FadeScreen.h"
#include "Core\Game.h"
#include "Core/Physics.h"

#ifdef _DEBUG
#include "DebugManager.h"
#endif

void SceneGlobal::Init(void)
{
	// リソースのロード
	VertexShader::Load("InstancingVS.hlsl");
	Texture::Load("background_summer_layer0");
	Texture::Load("background_summer_layer1");
	Texture::Load("background_summer_layer2");
	Texture::Load("background_winter_layer0");
	Texture::Load("background_winter_layer1");
	Texture::Load("background_winter_layer2");
	Texture::Load("sun_light");
	Texture::Load("00_Kuroko_face");
	Texture::Load("00_Kuroko_Face_Eye");
	Texture::Load("00_Kuroko_Fuku_BR_No_Sode");
	Texture::Load("00_Kuroko_hair");
	Sound::Load("game_stop");
	Sound::Load("stage_clear");
	Sound::Load("environment_summer");
	Sound::Load("environment_winter");
	Sound::Load("to_summer");
	Sound::Load("to_winter");
	Sound::Load("popup");
	Sound::Load("player_skill");
	Sound::LoadSerial("foot_mud", 6);
	Sound::LoadSerial("foot_snow", 6);
	Sound::LoadSerial("jump", 3);

	// 終了プロセスの設定
	Game::EndProcess = [this]
	{
		Sound::Get("game_stop")->Play();
		stop_timer.Reset(2.0f);
	};
	stop_timer.Reset(-1.0f);

	// グローバルカメラを作成
	camera = new Camera;

	// レンダースペースを作成し、カメラを追加
	RenderSpace::Add("global");
	RenderSpace::Get("global")->AddCamera(camera);

	// レンダーターゲットの作成
	render_target = RenderTarget::Make("rt_main");
	RenderSpace::Get("default")->GetCamera(0)->render_target = render_target;
	(new Object)->AddComponent<RectPolygon2D>(render_target, Layer::DEFAULT, "global");

	SetCameraActive(false);

	// フェイドエフェクトの初期化
	FadeScreen::Create();

	//重力設定
	Physics::GetInstance()->setGravity(Vector3(0.0f, -98.0f, 0.0f));

	// サウンドボリューム設定
	Sound::SetGlobalVolume(0.6f);


#ifdef _DEBUG
	DebugManager::Create();
#endif
}


void SceneGlobal::Update(void)
{
	if (stop_timer.interval > 0.0f)
	{
		stop_timer++;
		if (stop_timer.TimeUp())
			Game::Stop();
	}
}

void SceneGlobal::Uninit(void)
{
	Texture::Release("background_summer_layer0");
	Texture::Release("background_summer_layer1");
	Texture::Release("background_summer_layer2");
	Texture::Release("background_winter_layer0");
	Texture::Release("background_winter_layer1");
	Texture::Release("background_winter_layer2");
	Texture::Release("sun_light");
	Texture::Release("00_Kuroko_face");
	Texture::Release("00_Kuroko_Face_Eye");
	Texture::Release("00_Kuroko_Fuku_BR_No_Sode");
	Texture::Release("00_Kuroko_hair");
	Sound::Release("game_stop");
	Sound::Release("stage_clear");
	Sound::Release("environment_summer");
	Sound::Release("environment_winter");
	Sound::Release("to_summer");
	Sound::Release("to_winter");
	Sound::Release("popup");
	Sound::Release("player_skill");
	Sound::ReleaseSerial("foot_mud");
	Sound::ReleaseSerial("foot_snow");
	Sound::ReleaseSerial("jump");

	FadeScreen::Singleton<FadeScreen>::Destroy();

#ifdef _DEBUG
	DebugManager::Singleton<DebugManager>::Destroy();
#endif

}

void SceneGlobal::SetCameraActive(bool value)
{
	camera->SetActive(value);

	if (value)
	{
		RenderSpace::Get("default")->GetCamera(0)->render_target = render_target;
	}
	else
	{
		RenderSpace::Get("default")->GetCamera(0)->render_target = RenderTarget::BackBuffer();
	}
}
