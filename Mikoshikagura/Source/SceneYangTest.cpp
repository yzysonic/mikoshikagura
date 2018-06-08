#include "SceneYangTest.h"
#include "Core/Physics.h"
#include "Light.h"
#include "CameraPlay.h"
#include "CameraSmooth.h"
#include "FadeScreen.h"
#include "ImGuiCamera.h"
#include "DebugManager.h"

void SceneYangTest::Init(void)
{
	// リソースのロード
	ModelData::Load("field_summer");
	ModelData::Load("field_winter");
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");

	// 季節初期化
	SeasonManager::Create();
	SeasonManager::SetSeason(SeasonType::Summer);

	// オブジェクト初期化
	player	= new Player;
	player->AddComponent<ImGuiObject>();
	//player->SetPosition(Vector3(0.0f, 30.0f, 0.0f));
	
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
	
	debug	= DebugManager::GetInstance()->GetComponent<DebugMenu>();

	// カメラ初期化
	camera = new Camera;
	camera->transform.position = Vector3(0.0f, 20.0f, -60.0f);
	camera->setBackColor(Color(250, 250, 250, 255));
	camera->AddComponent<CameraPlay>();
	camera->AddComponent<CameraSmooth>(player);
	camera->AddComponent<ImGuiCamera>();
	
	// レンダリング設定
	Renderer::GetInstance()->setCamera(camera);
	Light::Init();

	// 重力設定
	Physics::GetInstance()->setGravity(Vector3(0.0f, -98.0f, 0.0f));
}

void SceneYangTest::Update(void)
{
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
	
	test[4]->transform.position.y = 10.0f*sinf(timer.Elapsed());
	test[5]->transform.position.y = 10.0f*cosf(timer.Elapsed());

	timer++;
}

void SceneYangTest::Uninit(void)
{
	Renderer::GetInstance()->setCamera(nullptr);

	ModelData::Release("field_summer");
	ModelData::Release("field_winter");
	Texture::Release("body_sum.tga");
	Texture::Release("misaki_head.tga");

	SeasonManager::Destroy();
}

SeasonTestObject::SeasonTestObject(void)
{
	model = AddComponent<StaticModel>("field_summer");
	collider = AddComponent<BoxCollider2D>();
	collider->size = Vector2::one*10.0f;
}

void SeasonTestObject::SetSummer(void)
{
	model->pData = ModelData::Get("field_summer");
}

void SeasonTestObject::SetWinter(void)
{
	model->pData = ModelData::Get("field_winter");
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

void SeasonTestObject::OnCollision(Object * object)
{
	collide = true;
}
