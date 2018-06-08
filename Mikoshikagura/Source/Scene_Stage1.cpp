#include "Scene_Stage1.h"
#include "FadeScreen.h"
#include "Core/Game.h"
#include "Light.h"
#include "ImGuiPlayer.h"
#include "DebugManager.h"
#include "Core\Physics.h"


void Scene_Stage1::Init(void)
{

	FadeScreen::FadeIn(Color::black, 0.0f);
	Light::Init();
	ModelData::Load("field_summer");
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");






	camera = new MainCamera;
	player = new Player;
	camera->SetTarget(&player->transform);


	Renderer::GetInstance()->setCamera(camera);

	player->SetPosition(Vector3(0, 70, 0));



	mapdata = new MapManager("Data/Map/prototype_map1.tmx");
	mapdata->CreateMapObject();
	mapdata->SetLayerActive(0, true);
	mapdata->SetPlayerpointer(player);

	Physics::GetInstance()->setGravity(Vector3(0.0f, -98.0f, 0.0f));
}

void Scene_Stage1::Update(void)
{
}

void Scene_Stage1::Uninit(void)
{
	Renderer::GetInstance()->setCamera(nullptr);
	Texture::Release("body_sum.tga");
	Texture::Release("misaki_head.tga");
	ModelData::Release("field_summer");

}
