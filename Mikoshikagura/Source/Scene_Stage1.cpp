#include "Scene_Stage1.h"
#include "FadeScreen.h"
#include "Core/Game.h"
#include "Light.h"
#include "SceneGlobal.h"

void Scene_Stage1::Init(void)
{
	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(true);

	// リソースのロード
	Texture::Load("map");
	Texture::Load("00_Kuroko_face");
	Texture::Load("00_Kuroko_Face_Eye");
	Texture::Load("00_Kuroko_Fuku_BR_No_Sode");
	Texture::Load("00_Kuroko_hair");
	Texture::Load("background_summer_layer0");
	Texture::Load("background_summer_layer1");
	Texture::Load("background_summer_layer2");
	Texture::Load("background_winter_layer0");
	Texture::Load("background_winter_layer1");
	Texture::Load("background_winter_layer2");
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
	Sound::Load("field_summer");
	Sound::Load("field_winter");
	//Sound::LoadSerial("foot_mud", 7);
	
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

	for(auto & light : sun_light)
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

	field_bgm_player = new FieldBgmPlayer;

	mapdata = new MapManager();
	mapdata->Load("Data/Map/prototype_map1.tmx");
	mapdata->SetPlayerpointer(player);
	//mapdata->SetSmoothPoint(camera);

	falling_snow = new FallingSnow(mapdata);
	light_particle = new ParticleOfLight(mapdata);

	mapdata->SetSignText(hukidashi);

	Light::Init();
	FadeScreen::FadeIn(Color::white, 1.0f);
}

void Scene_Stage1::Update(void)
{
}

void Scene_Stage1::Uninit(void)
{
	SeasonManager::Destroy();

	Renderer::GetInstance()->setCamera(nullptr);
	
	Texture::Release("map");
	Texture::Release("00_Kuroko_face");
	Texture::Release("00_Kuroko_Face_Eye");
	Texture::Release("00_Kuroko_Fuku_BR_No_Sode");
	Texture::Release("00_Kuroko_hair");
	Texture::Release("background_summer_layer0");
	Texture::Release("background_summer_layer1");
	Texture::Release("background_summer_layer2");
	Texture::Release("background_winter_layer0");
	Texture::Release("background_winter_layer1");
	Texture::Release("background_winter_layer2");
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
	Sound::Release("field_summer");
	Sound::Release("field_winter");
	//Sound::ReleaseSerial("foot_mud");

	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(false);
}
