#include "SceneGlobal.h"
#include "SceneTest.h"
#include "FadeScreen.h"
#include "Core\Game.h"

void SceneGlobal::Init(void)
{
	// �t�F�C�h�G�t�F�N�g�̏�����
	FadeScreen::Create();
	FadeScreen::FadeOut(Color::black, 0.0f);

	

	Texture::LoadTexture("magic_square");
}


void SceneGlobal::Update(void)
{
	if (GetKeyboardPress(DIK_RETURN))
		Game::Stop();
}

void SceneGlobal::Uninit(void)
{
	FadeScreen::Singleton<FadeScreen>::Destroy();
}