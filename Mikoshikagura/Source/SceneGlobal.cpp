#include "SceneGlobal.h"
#include "SceneTest.h"
#include "FadeScreen.h"
#include "Core\Game.h"

#ifdef _DEBUG
#include "DebugManager.h"
#endif

void SceneGlobal::Init(void)
{
	// �t�F�C�h�G�t�F�N�g�̏�����
	FadeScreen::Create();
	FadeScreen::FadeOut(Color::black, 0.0f);

	Texture::Load("magic_square");

#ifdef _DEBUG
	DebugManager::Create();
#endif
}


void SceneGlobal::Update(void)
{
}

void SceneGlobal::Uninit(void)
{
	FadeScreen::Singleton<FadeScreen>::Destroy();

#ifdef _DEBUG
	DebugManager::Singleton<DebugManager>::Destroy();
#endif

}