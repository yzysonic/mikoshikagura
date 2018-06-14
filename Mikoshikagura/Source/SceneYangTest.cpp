#include "SceneYangTest.h"
#include "Core/Physics.h"
#include "Light.h"
#include "FadeScreen.h"
#include "InspectorContentCamera.h"
#include "Inspector.h"
#include "DebugManager.h"
#include "FallingSnow.h"

#pragma region SceneYangTest

void SceneYangTest::Init(void)
{
	// リソースのロード
	ModelData::Load("field_summer");
	ModelData::Load("field_winter");
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");
	Texture::Load("mushroom");
	Texture::Load("particle");
	VertexShader::Load("InstancingVS.hlsl");
	Sound::Load("bgm_demo");

	// 季節初期化
	SeasonManager::Create(SeasonType::Summer);

	// オブジェクト初期化
	player	= new Player;
	player->SetPosition(Vector3(0.0f, 30.0f, 0.0f));

	item = new Item("mushroom", Texture::Get("mushroom")->size*0.1f);
	item->transform.position = Vector3(30.f, 15.f+ 0.5f*item->GetSize().y, 0.f);

	bgm_player = new SeasonBgmPlayerTest;
	bgm_player->Play();

	// カメラ初期化
	camera = new MainCamera;
	camera->SetTarget(&player->transform);
	camera->setBackColor(0xC1CED3);
	camera->GetComponent<CameraLimit>()->SetActive(false);

	// レンダリング設定
	Renderer::GetInstance()->setCamera(camera);
	Light::Init();

	// 重力設定
	Physics::GetInstance()->setGravity(Vector3(0.0f, -98.0f, 0.0f));

	// 地面ブロックの生成
	{
		int i = 0;
		for (auto& t : test)
		{
			t = new SeasonTestObject;
			t->transform.position.x = (float)(i*10);
			t->transform.position.y = (float)(((i/2)%2)*10);
			i++;
		}
	}

	snow = new FallingSnow;

	debug	= DebugManager::GetInstance()->GetComponent<DebugMenu>();
	DebugManager::OpenInspector(player);
}

void SceneYangTest::Update(void)
{
	timer++;

	test[4]->transform.position.y = 10.0f*sinf(timer.Elapsed());
	test[5]->transform.position.y = 10.0f*cosf(timer.Elapsed());

	// テストメニュー
	ImGui::SetNextWindowSize(ImVec2(150.0f, 0), ImGuiCond_Once);
	ImGui::Begin("TestMenu", NULL,  ImGuiWindowFlags_NoResize);

	switch (SeasonManager::GetSeason())
	{
	case SeasonType::Summer:
		if (ImGui::Button("SetToWinter", ImVec2(-1.0f, 0.0f))) { SeasonManager::SetSeason(SeasonType::Winter); }
		break;

	case SeasonType::Winter:
		if (ImGui::Button("SetToSummer", ImVec2(-1.0f, 0.0f))) { SeasonManager::SetSeason(SeasonType::Summer); }
		break;
	}

	ImGui::End();

	// デバッグメニュー
	ImGui::SetNextWindowSize(ImVec2(debug->MenuWidth, 0), ImGuiCond_Once);
	ImGui::Begin("DebugMenu ", NULL, ImGuiWindowFlags_NoResize);
	debug->GuiContent();
	ImGui::End();
}

void SceneYangTest::Uninit(void)
{
	Renderer::GetInstance()->setCamera(nullptr);

	ModelData::Release("field_summer");
	ModelData::Release("field_winter");
	Texture::Release("body_sum.tga");
	Texture::Release("misaki_head.tga");
	Texture::Release("mushroom");
	Texture::Release("snow");
	Sound::Release("bgm_demo");

	SeasonManager::Destroy();
}

#pragma endregion


#pragma region SeasonTestObject

SeasonTestObject::SeasonTestObject(void)
{
	type = ObjectType::Field;
	model = AddComponent<SeasonModel>("field");
	collider = AddComponent<BoxCollider2D>();
	collider->size = Vector2::one*10.0f;
}

void SeasonTestObject::Update(void)
{
	ImGui::Begin("BoxCollision");

	if (collide)
		ImGui::Text("atari!");
	else
		ImGui::Text("none");

	ImGui::End();

	collide = false;
}

void SeasonTestObject::OnCollisionEnter(Object * other)
{
	SwitchModel();
}

void SeasonTestObject::OnCollisionStay(Object * object)
{
	collide = true;
}

void SeasonTestObject::OnCollisionExit(Object * other)
{
	SwitchModel();
}

void SeasonTestObject::SwitchModel(void)
{
	if (model->GetSeason() == SeasonType::Summer)
		model->SetWinter();
	else
		model->SetSummer();
}
#pragma endregion

SeasonBgmPlayerTest::SeasonBgmPlayerTest(void)
{
	player = AddComponent<SoundPlayer>("bgm_demo");
}

void SeasonBgmPlayerTest::SetSummer(void)
{
	player->FadeVolume(1.0f, 1.5f);
}

void SeasonBgmPlayerTest::SetWinter(void)
{
	player->FadeVolume(0.3f, 1.5f);
}

void SeasonBgmPlayerTest::Play(void)
{
	player->Play();
}
