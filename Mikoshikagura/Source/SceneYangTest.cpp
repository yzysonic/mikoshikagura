#include "SceneYangTest.h"
#include "CameraPlay.h"
#include "FadeScreen.h"
#include "Light.h"

void SceneYangTest::Init(void)
{

	ModelData::Load("field");
	
	test = new Object;
	test->AddComponent<StaticModel>("field");
	test->transform.scale = Vector3::one*0.1f;

	camera = new Camera;
	camera->setBackColor(Color(250, 250, 250, 255));
	camera->AddComponent<CameraPlay>()->SetActive(true);
	
	Renderer::GetInstance()->setCamera(camera);
	Light::Init();
}

void SceneYangTest::Uninit(void)
{
	Renderer::GetInstance()->setCamera(nullptr);
	ModelData::Release("field");
}
