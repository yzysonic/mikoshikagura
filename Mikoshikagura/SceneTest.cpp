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

	//test = new Object;
	//test->AddComponent<RectPolygon>("magic_square")->SetSize(Vector2(100, 100));
	//test->transform.scale = Vector3::one*10.f;
	//test->transform.position.z = -100.0f;


	//testBG = new Object;
	//Texture::LoadTexture("testbg");
	//testBG->AddComponent<RectPolygon>("testbg")->SetSize(Vector2(1000, 1000));
	//testBG->transform.scale = Vector3::one*10.f;



	Texture::LoadTexture("target");

	target1 = new Object;
	target1->AddComponent<RectPolygon>("target");
	target1->transform.scale = Vector3::one*20.f;
	target1->transform.position = Vector3(100, 20, 0);
	target2 = new Object;
	target2->AddComponent<RectPolygon>("target");
	target2->transform.scale = Vector3::one*20.f;
	target2->transform.position = Vector3(150, 25, 0);

	camera = new MainCamera;
	player = new Player;

	camera->SetTarget(&player->transform);

	camera->AddSnapper(&target1->transform);
	camera->AddSnapper(&target2->transform);
	//camera->RemoveSnapper(&target1->transform);

	Renderer::GetInstance()->setCamera(camera);




	//0522nagai mapdataì¬



}

void SceneTest::Update(void)
{
	//test->transform.position.x += 0.3f;
	//test->transform.rotate(0.05f,0,0);

}

void SceneTest::Uninit(void)
{
}
