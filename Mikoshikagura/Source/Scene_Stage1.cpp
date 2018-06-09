#include "Scene_Stage1.h"
#include "FadeScreen.h"
#include "Core/Game.h"
#include "Light.h"


void Scene_Stage1::Init(void)
{
	// リソースのロード
	ModelData::Load("field_summer");
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");
	
	// ゲームオブジェクトの初期化
	player = new Player;
	player->SetPosition(Vector3(0, 70, 0));

	camera = new MainCamera;
	camera->SetTarget(&player->transform);
	Renderer::GetInstance()->setCamera(camera);

	mapdata = new MapManager("Data/Map/prototype_map1.tmx");
	mapdata->CreateMapObject();
	mapdata->SetLayerActive(0, true);
	mapdata->SetPlayerpointer(player);

	Light::Init();
	FadeScreen::FadeIn(Color::black, 0.0f);
}

void Scene_Stage1::Update(void)
{
}

void Scene_Stage1::Uninit(void)
{
	Renderer::GetInstance()->setCamera(nullptr);
	
	ModelData::Release("field_summer");
	Texture::Release("body_sum.tga");
	Texture::Release("misaki_head.tga");
}
