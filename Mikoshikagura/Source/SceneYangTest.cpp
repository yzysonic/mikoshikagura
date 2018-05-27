#include "SceneYangTest.h"
#include "Light.h"
#include "CameraPlay.h"
#include "CameraSmooth.h"
#include "FadeScreen.h"
#include "ImGuiCamera.h"

void SceneYangTest::Init(void)
{

	ModelData::Load("field_summer");
	ModelData::Load("field_winter");
	Texture::Load("body_sum.tga");
	Texture::Load("misaki_head.tga");

	player = new Player;
	
	test = new Object;
	test->AddComponent<StaticModel>("field_summer");

	camera = new Camera;
	camera->setBackColor(Color(250, 250, 250, 255));
	camera->AddComponent<CameraPlay>()->SetActive(true);
	camera->AddComponent<CameraSmooth>(player);
	camera->AddComponent<ImGuiCamera>();

	season_state = SeasonType::Summer;
	
	Renderer::GetInstance()->setCamera(camera);
	Light::Init();
}

void SceneYangTest::Update(void)
{
	ImGui::SetNextWindowSize(ImVec2(150.0f, 0), ImGuiCond_Once);

	ImGui::Begin("TestMenu", NULL,  ImGuiWindowFlags_NoResize);

	if (ImGui::Button("SeasonChange", ImVec2(-1.0f,0.0f)))
	{
		switch (season_state)
		{
		case SeasonType::Summer:
			test->GetComponent<StaticModel>()->pData = ModelData::Get("field_winter");
			season_state = SeasonType::Winter;
			break;
		
		case SeasonType::Winter:
			test->GetComponent<StaticModel>()->pData = ModelData::Get("field_summer");
			season_state = SeasonType::Summer;
			break;
		}
	}

	ImGui::End();
}

void SceneYangTest::Uninit(void)
{
	Renderer::GetInstance()->setCamera(nullptr);
	ModelData::Release("field_summer");
	ModelData::Release("field_winter");
}
