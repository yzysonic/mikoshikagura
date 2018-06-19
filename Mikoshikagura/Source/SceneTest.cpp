#include "SceneTest.h"
#include "FadeScreen.h"
#include "Light.h"
#include "DebugManager.h"
#include "Core\Physics.h"
#include "SeasonManager.h"
void SceneTest::Init(void)
{

	SeasonManager::Create();
	SeasonManager::SetSeason(SeasonType::Summer);

	FadeScreen::FadeIn(Color::white, 1.0f);
	Light::Init();
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");

	Texture::Load("target");
	Texture::Load("hukidashi");
	// リソースのロード
	Texture::Load("map");
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");
	Texture::Load("background");

	ModelData::Load("Maptip/20_summer");
	ModelData::Load("Maptip/20_winter");
	ModelData::Load("Maptip/23");
	ModelData::Load("Maptip/37");
	ModelData::Load("field_summer");
	Sound::Load("bgm_demo");



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
	mapdata = new MapManager();
	mapdata->Load("Data/Map/prototype_map1.tmx");
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

	//0613umeda GoalObject
	goal = new GoalObject<SceneTest>();
	goal->transform.scale = Vector3::one*10.f;
	goal->transform.position = Vector3(200, 60, 3);
	goal->AddComponent<BoxCollider2D>()->size = Vector2(10, 10);
}

void SceneTest::Update(void)
{
	//test->transform.position.x += 0.3f;
	//test->transform.rotate(0.05f,0,0);
}

void SceneTest::Uninit(void)
{
	Renderer::GetInstance()->setCamera(nullptr);
	Texture::Release("body_sum.tga");
	Texture::Release("misaki_head.tga");

	Texture::Release("target");
	Texture::Release("hukidashi");
	// リソースのロード
	Texture::Release("map");
	Texture::Release("body_sum.tga");
	Texture::Release("misaki_head.tga");
	Texture::Release("background");

	ModelData::Release("Maptip/20_summer");
	ModelData::Release("Maptip/20_winter");
	ModelData::Release("Maptip/23");
	ModelData::Release("Maptip/37");
	ModelData::Release("field_summer");
	Sound::Release("bgm_demo");
}
