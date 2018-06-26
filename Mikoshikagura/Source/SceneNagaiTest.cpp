#include "SceneNagaiTest.h"

void SceneNagaiTest::Init(void)
{

	Texture::Load("sun_light2");
	stage1.Init();

	for (auto itr  : sunlight) {
		itr = new SunLight();
	}

}

void SceneNagaiTest::Update(void)
{
	stage1.Update();
}

void SceneNagaiTest::Uninit(void)
{
	stage1.Uninit();
	Texture::Release("sun_light2");
}
