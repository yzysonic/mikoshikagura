#include "SceneGlobal.h"
#include "SceneTest.h"
#include "FadeScreen.h"
#include "Core\Game.h"
#include "Core/Physics.h"

#ifdef _DEBUG
#include "DebugManager.h"
#endif

void SceneGlobal::Init(void)
{
	VertexShader::Load("InstancingVS.hlsl");

	// �O���[�o���J�������쐬
	camera = new Camera;

	// �����_�[�X�y�[�X���쐬���A�J������ǉ�
	RenderSpace::Add("global");
	RenderSpace::Get("global")->AddCamera(camera);

	// �����_�[�^�[�Q�b�g�̍쐬
	render_target = RenderTarget::Make("rt_main");
	RenderSpace::Get("default")->GetCamera(0)->render_target = render_target;
	(new Object)->AddComponent<RectPolygon2D>(render_target, Layer::DEFAULT, "global");

	SetCameraActive(false);

	// �t�F�C�h�G�t�F�N�g�̏�����
	FadeScreen::Create();

	//�d�͐ݒ�
	Physics::GetInstance()->setGravity(Vector3(0.0f, -98.0f, 0.0f));


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

void SceneGlobal::SetCameraActive(bool value)
{
	camera->SetActive(value);

	if (value)
	{
		RenderSpace::Get("default")->GetCamera(0)->render_target = render_target;
	}
	else
	{
		RenderSpace::Get("default")->GetCamera(0)->render_target = RenderTarget::BackBuffer();
	}
}
