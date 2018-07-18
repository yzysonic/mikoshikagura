#include "Light.h"
#include "Core/Direct3D.h"

D3DLIGHT9 Light::light[NUM_LIGHT];
smart_ptr<Light> Light::instance;

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void Light::Init(void)
{
	if (instance == nullptr)
		instance.reset(new Light);

	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&light[0], sizeof(D3DLIGHT9));

	// ���C�g0�̃^�C�v�̐ݒ�
	light[0].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g0�̊g�U���̐ݒ�
	light[0].Diffuse = D3DXCOLOR(0xffffffff);
	light[0].Ambient = D3DXCOLOR(0xffffffff);
	
	// ���C�g0�̕����̐ݒ�
	light[0].Direction = Vector3(968.0f, -934.0f, 2689.0f).normalized();

	// ���C�g0�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(0, &light[0]);

	// ���C�g0���g�p�g�p��Ԃ�
	pDevice->LightEnable(0, TRUE);


	// ���C�e�B���O���[�h��ON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(0xffffffff));
	//pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(0xfff6cb6e));
}

Light* Light::GetInstance(void)
{
	return instance.get();
}

HRESULT Light::OnLostDevice(void)
{
	return S_OK;
}

HRESULT Light::OnResetDevice(void)
{
	Init();
	return S_OK;
}
