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
	ModelData::Load("Maptip/23");
	ModelData::Load("Maptip/37");
	ModelData::Load("field_summer");
	
	// ゲームオブジェクトの初期化
	player = new Player;
	player->SetPosition(Vector3(0, 70, 0));

	camera = new MainCamera;
	camera->SetTarget(&player->transform);
	camera->setBackColor(Color(250, 250, 250, 255));
	Renderer::GetInstance()->setCamera(camera);

	mapdata = new MapManager();
	mapdata->Load("Data/Map/prototype_map1.tmx");
	mapdata->SetPlayerpointer(player);

	background = new Background;

	Light::Init();
	FadeScreen::FadeIn(Color::black, 0.0f);
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
	ModelData::Release("20");
	ModelData::Release("23");
	ModelData::Release("37");
	ModelData::Release("field_summer");
}
