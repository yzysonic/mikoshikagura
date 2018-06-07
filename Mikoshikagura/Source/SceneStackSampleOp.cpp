#include "SceneStackSampleOp.h"
#include "FadeScreen.h"

void SceneStackSampleOp::Init(void)
{
	// レンダーターゲットの作成
	render_target = RenderTarget::Make("rt_op");

	// レンダーターゲットを指定
	auto camera = RenderSpace::Get("default")->GetCamera(0);
	last_render_target = camera->render_target;
	camera->render_target = render_target;

	render_target_polygon = (new Object)->AddComponent<RectPolygon2D>(render_target, Layer::MASK, "global");

	// シーンの初期化

	Texture::Load("scene_stack_sample_op");

	image = new Object;
	image->AddComponent<RectPolygon2D>("scene_stack_sample_op");
	
	state = State::WaitInput;
	timer.Reset(1.0f);
}

void SceneStackSampleOp::Update(void)
{
	switch (state)
	{
	case State::WaitInput:
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			state = State::FadeOut;
		}
		break;

	case State::FadeOut:
		auto opacity = Lerpf(1.0f, 0.0f, timer.Progress());
		render_target_polygon->SetOpacity(opacity);
		if (timer.Progress() >= 1.0f)
		{
			GameManager::GetInstance()->PopScene();
		}
		timer++;
		break;
	}
}

void SceneStackSampleOp::Uninit(void)
{
	if (last_render_target)
	{
		RenderSpace::Get("default")->GetCamera(0)->render_target = last_render_target;
	}
	RenderTarget::Release("rt_op");
	Texture::Release("scene_stack_sample_op");
}
