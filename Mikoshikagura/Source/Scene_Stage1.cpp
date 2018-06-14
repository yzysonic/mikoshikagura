#include "Scene_Stage1.h"
#include "FadeScreen.h"
#include "Core/Game.h"
#include "Light.h"


void Scene_Stage1::Init(void)
{
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
	
	Light::Init();
	SeasonManager::Create(SeasonType::Summer);

	// ゲームオブジェクトの初期化
	background = new Background;

	player = new Player;
	player->SetPosition(Vector3(10, 70, 0));

	camera = new MainCamera;
	camera->SetTarget(&player->transform);
	camera->setBackColor(Color(250, 250, 250, 255));
	Renderer::GetInstance()->setCamera(camera);

	mapdata = new MapManager();
	mapdata->Load("Data/Map/prototype_map1.tmx");
	mapdata->SetPlayerpointer(player);

	wall = new Object;
	wall->type = ObjectType::Field;
	wall->AddComponent<BoxCollider2D>();
	wall->GetComponent<BoxCollider2D>()->size = Vector2(10.0f, 1000.0f);
	wall->GetComponent<BoxCollider2D>()->SetActive(true);

	FadeScreen::FadeIn(Color::white, 1.0f);

	Sound::Get("bgm_demo")->Play();
}

void Scene_Stage1::Update(void)
{

}

void Scene_Stage1::Uninit(void)
{
	Renderer::GetInstance()->setCamera(nullptr);
	
	Texture::Release("map");
	Texture::Release("body_sum.tga");
	Texture::Release("misaki_head.tga");
	Texture::Release("background");
	ModelData::Release("Maptip/23");
	ModelData::Release("Maptip/37");
	ModelData::Release("Maptip/20_summer");
	ModelData::Release("Maptip/20_winter");
	ModelData::Release("field_summer");
	Sound::Release("bgm_demo");
}
