#include "SceneTest.h"
#include "FadeScreen.h"
#include "Core\Game.h"
#include "Light.h"

void SceneTest::Init(void)
{

	FadeScreen::FadeIn(Color::black, 0.0f);
	Light::Init();

	Texture::LoadTexture("body_sum", "body_sum.tga");
	Texture::LoadTexture("misaki_head", "misaki_head.tga");

	test = new Object;
	test->AddComponent<RectPolygon>("magic_square");
	test->transform.scale = Vector3::one*100.f;
	//test->transform.position.z = -100.0f;


	testBG = new Object;
	Texture::LoadTexture("testbg");
	testBG->AddComponent<RectPolygon>("testbg");
	testBG->transform.scale = Vector3::one*2000.f;



	camera = new MainCamera;
	player = new Player;

	Renderer::GetInstance()->setCamera(camera);
}

void SceneTest::Update(void)
{
	test->transform.position.x += 0.3f;
	test->transform.rotate(0.05f,0,0);

}

void SceneTest::Uninit(void)
{
}
