#pragma once
#include "Core\Core.h"

class SceneGlobal : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void SetCameraActive(bool value);

private:
	Camera* camera;
	RenderTarget* render_target;
	FrameTimer stop_timer;
};