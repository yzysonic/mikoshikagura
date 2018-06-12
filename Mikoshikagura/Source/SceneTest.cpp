#include "SceneTest.h"
#include "FadeScreen.h"
#include "Core/Game.h"
#include "Light.h"
#include "DebugManager.h"
#include "Core\Physics.h"


void SceneTest::Init(void)
{

	FadeScreen::FadeIn(Color::black, 0.0f);
	Light::Init();
	ModelData::Load("field_summer");
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");

	Texture::Load("target");
	ModelData::Load("field");

	target1 = new Object;
	target1->AddComponent<RectPolygon>("target");
	target1->transform.scale = Vector3::one*20.f;
	target1->transform.position = Vector3(100, 20, 100);
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

	player->SetPosition(Vector3(0, 70, 0));



	//0522nagai mapdataì¬
	mapdata = new MapManager();
	mapdata->Load("Data/Map/prototype_map1.tmx");
	mapdata->CreateMapObject();
	mapdata->SetLayerActive(0, true);
	mapdata->SetPlayerpointer(player);

	Physics::GetInstance()->setGravity(Vector3(0.0f, -98.0f, 0.0f));
}

void SceneTest::Update(void)
{
	//test->transform.position.x += 0.3f;
	//test->transform.rotate(0.05f,0,0);



}

void SceneTest::Uninit(void)
{
	Renderer::GetInstance()->setCamera(nullptr);
	ModelData::Release("field");

}
