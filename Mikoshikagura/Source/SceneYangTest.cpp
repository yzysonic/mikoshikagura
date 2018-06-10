#include "SceneYangTest.h"
#include "Core/Physics.h"
#include "Light.h"
#include "FadeScreen.h"
#include "InspectorContentCamera.h"
#include "Inspector.h"
#include "DebugManager.h"

#pragma region SceneYangTest

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
	player->SetPosition(Vector3(0.0f, 30.0f, 0.0f));

	// カメラ初期化
	camera = new MainCamera;
	camera->SetTarget(&player->transform);

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

	SeasonManager::Destroy();
}

#pragma endregion


#pragma region SeasonTestObject

SeasonTestObject::SeasonTestObject(void)
{
	type = ObjectType::Field;
	model = AddComponent<StaticModel>("field_summer");
	collider = AddComponent<BoxCollider2D>();
	collider->size = Vector2::one*10.0f;
	summer_model = ModelData::Get("field_summer");
	winter_model = ModelData::Get("field_winter");
}

void SeasonTestObject::SetSummer(void)
{
	model->pData = summer_model;
}

void SeasonTestObject::SetWinter(void)
{
	model->pData = winter_model;
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
	if (model->pData == summer_model)
		model->pData = winter_model;
	else
		model->pData = summer_model;
}

#pragma endregion