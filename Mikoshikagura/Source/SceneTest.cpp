#include "SceneTest.h"
#include "FadeScreen.h"
#include "Core/Game.h"
#include "Light.h"
#include "SceneGlobal.h"
void SceneTest::Init(void)
{
	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(true);

	// リソースのロード
	VertexShader::Load("NormalVS.hlsl");
	PixelShader::Load("OverlayPS.hlsl");

	Texture::Load("map");
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");
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
	Sound::Load("bgm_demo");
	Sound::LoadSerial("foot_mud", 7);

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

	mapdata = new MapManager();
	mapdata->Load("Data/Map/prototype_map2.tmx");
	mapdata->SetPlayerpointer(player);

	background = new Background;
	falling_snow = new FallingSnow(mapdata);
	light_particle = new ParticleOfLight(mapdata);

	hukidashi = new Hukidashi;
	mapdata->SetSignText(hukidashi);

	Light::Init();
	FadeScreen::FadeIn(Color::black, 0.0f);

	wall = new Object;
	wall->type = ObjectType::Field;
	wall->AddComponent<BoxCollider2D>()->size = Vector2(10.0f, 1000.0f);

	goal = new GoalObject<SceneTest>();
	goal->transform.scale = Vector3::one*10.f;
	goal->transform.position = Vector3(380, 0, 0);
	goal->AddComponent<BoxCollider2D>()->size = Vector2(10, 1000);

	snapper = new Snapper();
	snapper->transform.position = Vector3(185, 95, 0);
	snapper->AddComponent<BoxCollider2D>()->size = Vector2(40, 60);
	snapper->GetComponent<BoxCollider2D>()->offset = Vector2(-20, -30);
	camera->AddSnapper(snapper);

	FadeScreen::FadeIn(Color::white, 1.0f);

	Sound::Get("bgm_demo")->Play();
}

void SceneTest::Update(void)
{
	//test->transform.position.x += 0.3f;
	//test->transform.rotate(0.05f,0,0);
}

void SceneTest::Uninit(void)
{
	SeasonManager::Destroy();

	Renderer::GetInstance()->setCamera(nullptr);

	Texture::Release("map");
	Texture::Release("body_sum.tga");
	Texture::Release("misaki_head.tga");
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
	Sound::Release("bgm_demo");
	Sound::ReleaseSerial("foot_mud");

	((SceneGlobal*)GameManager::GetInstance()->GetGlobalScene())->SetCameraActive(false);
}
