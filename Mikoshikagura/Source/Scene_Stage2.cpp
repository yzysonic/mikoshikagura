#include "Scene_Stage2.h"
#include "FadeScreen.h"
#include "Core/Game.h"
#include "Light.h"
#include "SceneGlobal.h"

void Scene_Stage2::Init(void)
{
	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(true);

	// リソースのロード
	Texture::Load("map");
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");
	Texture::Load("bark01_bottom.tga");			//木
	Texture::Load("branch01.tga");
	Texture::Load("particle");
	Texture::Load("particle_light");
	Texture::Load("hukidashi");
	Texture::Load("Maptip/45");
	Texture::Load("sun_light");
	Texture::Load("sun_light2");

	ModelData::Load("Maptip/20_summer");
	ModelData::Load("Maptip/20_winter");
	ModelData::Load("Maptip/23");
	ModelData::Load("Maptip/37");
	ModelData::Load("field_summer");
	ModelData::Load("tree_tekito");
	ModelData::Load("torii");
	Sound::Load("field_summer");
	Sound::Load("field_winter");

	Light::Init();
	SeasonManager::Create(SeasonType::Summer);

	// ゲームオブジェクトの初期化
	player = new Player;
	player->SetPosition(Vector3(10, 70, 0));

	camera = new MainCamera;
	camera->render_target = RenderTarget::Get("rt_main");
	camera->SetTarget(&player->transform);
	camera->setBackColor(Color(250, 250, 250, 255));
	RenderSpace::Get("default")->SetCamera(0, camera);

	for (auto & light : sun_light)
		light = new SunLight;

	wall = new Object;
	wall->type = ObjectType::Field;
	wall->AddComponent<BoxCollider2D>();
	wall->GetComponent<BoxCollider2D>()->size = Vector2(10.0f, 1000.0f);
	wall->GetComponent<BoxCollider2D>()->SetActive(true);

	goal = new GoalObject<SceneTitle>();
	goal->transform.scale = Vector3::one*10.f;
	goal->transform.position = Vector3(380, 0, 0);
	goal->AddComponent<BoxCollider2D>()->size = Vector2(10, 1000);

	background = new Background;
	hukidashi = new Hukidashi;

	field_bgm_player = new SeasonSoundPlayer("field");
	environment_sound_player = new SeasonSoundPlayer("environment");

	mapdata = new MapManager();
	mapdata->Load("Data/Map/prototype_map2_new.tmx");
	mapdata->SetPlayerpointer(player);
	mapdata->SetSmoothPoint(camera);

	falling_snow = new FallingSnow(mapdata);
	light_particle = new ParticleOfLight(mapdata);

	mapdata->SetSignText(hukidashi);

	Light::Init();
	FadeScreen::FadeIn(Color::white, 1.0f);
}

void Scene_Stage2::Update(void)
{
}

void Scene_Stage2::Uninit(void)
{
	SeasonManager::Destroy();

	Renderer::GetInstance()->setCamera(nullptr);

	Texture::Release("map");
	Texture::Release("body_sum.tga");
	Texture::Release("misaki_head.tga");
	Texture::Release("particle");
	Texture::Release("particle_light");
	Texture::Release("hukidashi");
	Texture::Release("sun_light");
	Texture::Release("sun_light2");
	ModelData::Release("Maptip/23");
	ModelData::Release("Maptip/37");
	ModelData::Release("Maptip/20_summer");
	ModelData::Release("Maptip/20_winter");
	ModelData::Release("field_summer");
	ModelData::Release("torii");
	ModelData::Release("tree_tekito");
	Sound::Release("field_summer");
	Sound::Release("field_winter");

	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(false);
}
