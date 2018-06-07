#pragma once
#include "Core\Core.h"
#include "SceneYangTest.h"

class SceneStackSampleGame : public Scene
{
	enum class State
	{
		Capture,
		Opening,
		Run
	};
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	Camera* camera;
	Object* image;
	RectPolygon2D* render_target_polygon;
	RenderTarget* render_target;
	State state;
	SceneYangTest* scene_test;
};