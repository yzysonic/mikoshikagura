#include "SceneGlobal.h"
#include "SceneTest.h"
#include "FadeScreen.h"
#include "Core\Game.h"

#ifdef _DEBUG
#include "DebugManager.h"
#endif

void SceneGlobal::Init(void)
{
	// グローバルカメラを作成
	camera = new Camera;

	// レンダースペースを作成し、カメラを追加
	RenderSpace::Add("global");
	RenderSpace::Get("global")->AddCamera(camera);

	//// レンダーターゲットの作成
	render_target = RenderTarget::Make("rt_main");
	RenderSpace::Get("default")->GetCamera(0)->render_target = render_target;
	(new Object)->AddComponent<RectPolygon2D>(render_target, Layer::DEFAULT, "global");

	SetCameraActive(false);

	// フェイドエフェクトの初期化
	FadeScreen::Create();


#ifdef _DEBUG
	DebugManager::Create();
#endif
}


void SceneGlobal::Update(void)
{
}

void SceneGlobal::Uninit(void)
{
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
