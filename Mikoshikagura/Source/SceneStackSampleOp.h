#pragma once
#include "Core\Core.h"

class SceneStackSampleOp : public Scene
{
private:
	static constexpr float FadeTime = 0.3f;

	enum class State
	{
		FadeOut,
		WaitInput
	};

public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
	RenderTarget* last_render_target;

private:
	RenderTarget* render_target;
	Object* image;
	RectPolygon2D* render_target_polygon;
	State state;
	FrameTimer timer;
};