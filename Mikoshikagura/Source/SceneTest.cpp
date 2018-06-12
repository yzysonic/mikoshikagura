#include "SceneTest.h"
#include "FadeScreen.h"
#include "Light.h"
#include "DebugManager.h"
#include "Core\Physics.h"

void SceneTest::Init(void)
{

	FadeScreen::FadeIn(Color::white, 1.0f);
	Light::Init();
	ModelData::Load("field_summer");
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");

	//test = new Object;
	//test->AddComponent<RectPolygon>("magic_square")->SetSize(Vector2(100, 100));
	//test->transform.scale = Vector3::one*10.f;
	//test->transform.position.z = -100.0f;


	//testBG = new Object;
	//Texture::Load("testbg");
	//testBG->AddComponent<RectPolygon>("testbg")->SetSize(Vector2(1000, 1000));
	//testBG->transform.scale = Vector3::one*10.f;



	Texture::Load("target");
	Texture::Load("hukidashi");
	ModelData::Load("field");

	target1 = new Object;
	target1->AddComponent<RectPolygon>("target");
	target1->transform.scale = Vector3::one*20.f;
	target1->transform.position = Vector3(55, 65, 3);
	target2 = new Object;
	target2->AddComponent<RectPolygon>("target");
	target2->transform.scale = Vector3::one*20.f;
	target2->transform.position = Vector3(120, 65, 3);

	camera = new MainCamera;
	player = new Player;
	camera->SetTarget(&player->transform);

	camera->AddSnapper(&target1->transform);
	camera->AddSnapper(&target2->transform);
	//camera->RemoveSnapper(&target1->transform);

	Renderer::GetInstance()->setCamera(camera);

	player->SetPosition(Vector3(0, 70, 0));



	//0522nagai mapdata作成
	mapdata = new MapManager("Data/Map/prototype_map1.tmx");
	mapdata->CreateMapObject();
	mapdata->SetLayerActive(0, true);
	mapdata->SetPlayerpointer(player);

	Physics::GetInstance()->setGravity(Vector3(0.0f, -98.0f, 0.0f));

	//0601umeda hukidashi作成
	hukidashi = new Hukidashi;

	kanban = new Sign("これはテストメッセージです。\nThis is test message.", hukidashi);
	kanban->AddComponent<RectPolygon>("target");
	kanban->transform.scale = Vector3::one*10.f;
	kanban->transform.position = Vector3(180, 60, 3);
	kanban->AddComponent<BoxCollider2D>()->size = Vector2(10,10);


	kanban2 = new Sign("メッセージ遷移テスト\nThis is test message.", hukidashi);
	kanban2->AddComponent<RectPolygon>("target");
	kanban2->transform.scale = Vector3::one*10.f;
	kanban2->transform.position = Vector3(170, 60, 3);
	kanban2->AddComponent<BoxCollider2D>()->size = Vector2(10, 10);
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
