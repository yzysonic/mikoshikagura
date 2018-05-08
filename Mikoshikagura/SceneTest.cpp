#include "SceneTest.h"
#include "FadeScreen.h"
#include "Core\Game.h"

void SceneTest::Init(void)
{

	FadeScreen::FadeIn(Color::black, 0.0f);

	test = new Object;
	test->AddComponent<RectPolygon>("magic_square");
	test->transform.scale = Vector3::one*100.f;
	test->transform.position.z = -100.0f;
}

void SceneTest::Update(void)
{
	//test->transform.position.x += 0.3f;
	test->transform.rotate(0.05f,0,0.01f);

}

void SceneTest::Uninit(void)
{
}
