#include "Renderer.h"
#include "Direct3D.h"
#include "RenderSpace.h"

#ifdef IMGUI
#include "../Imgui/ImGuiImpl.h"
#endif

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
char g_DebugText[40][256] = {};

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void DrawDebug();


//=============================================================================
// ������
//=============================================================================
void Renderer::Create(void)
{
	Singleton::Create();

	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// �J�����O���s��Ȃ�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);					// Z�o�b�t�@���g�p
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DX_FILTER_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���X�e�[�W���Z��������
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// �Q�Ԗڂ̃A���t�@����

	// �����`����
	RenderSpace::Add("default");

	// �`�撆�J�����̐ݒ�
	m_pInstance->fixedCamera = new Camera;
	m_pInstance->drawing_camera = m_pInstance->fixedCamera;
	m_pInstance->setCamera(nullptr);

}

//=============================================================================
// �I������
//=============================================================================
void Renderer::Destroy(void)
{
	if (m_pInstance == nullptr)
		return;

	for (int i = RenderSpace::RenderSpaceCount(); i >=0; i--)
		RenderSpace::Delete(i);

	Singleton::Destroy();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Renderer::DrawFrame()
{
	auto pDevice = Direct3D::GetDevice();

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		// ���C���`�揈��
		RenderSpace::Draw();

		// �f�o�b�O
		DrawDebug();

#ifdef IMGUI
		// ImGui�̕`��
		ImGuiImpl::Draw();
#endif

		// Direct3D�ɂ��`��̏I��
		pDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	HRESULT hr;
	hr = pDevice->Present(NULL, NULL, NULL, NULL);

	// �f�o�C�X���X�g�̌��m
	if (hr == D3DERR_DEVICELOST) {

		// ���A�\�̏ꍇ
		if (pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			Direct3D::ResetDevice();
		}
	}
}

//=============================================================================
// �J�����擾
//=============================================================================
Camera * Renderer::getCamera(void)
{
	return m_pInstance->drawing_camera;
}
Camera * Renderer::getCamera(std::string render_space_name, int no)
{
	return RenderSpace::Get(render_space_name)->GetCamera(no);
}
Camera * Renderer::getCamera(int render_space_no, int no)
{
	return RenderSpace::Get(render_space_no)->GetCamera(no);
}


//=============================================================================
// �J�����ݒu
//=============================================================================
void Renderer::setCamera(Camera * camera, std::string render_space)
{
	if (RenderSpace::Get(render_space)->CameraCount() > 0)
		RenderSpace::Get(render_space)->RemoveCamera(RenderSpace::Get(render_space)->GetCamera(0));

	if (camera != nullptr)
		RenderSpace::Get(render_space)->AddCamera(camera);
	else
		RenderSpace::Get(render_space)->AddCamera(fixedCamera);
}
void Renderer::setCamera(Camera * camera, int render_space)
{
	if(RenderSpace::Get(render_space)->CameraCount() > 0)
		RenderSpace::Get(render_space)->RemoveCamera(RenderSpace::Get(render_space)->GetCamera(0));

	if (camera != nullptr)
		RenderSpace::Get(render_space)->AddCamera(camera);
	else
		RenderSpace::Get(render_space)->AddCamera(fixedCamera);
}



//=============================================================================
// �f�o�b�O������̎擾
//=============================================================================
char *GetDebugText(int line)
{
	return g_DebugText[line];
}

//=============================================================================
// �f�o�b�O�����̕`��
//=============================================================================
void DrawDebug()
{
	RECT rect;
	auto pDevice = Direct3D::GetDevice();
	pDevice->SetRenderTarget(0, RenderTarget::BackBuffer()->pSurface);
	pDevice->SetDepthStencilSurface(RenderTarget::BackBuffer()->pDepthSurface);

	for (int i = 0; i < 40; i++)
	{
		rect = { 0,i * 20,SystemParameters::ResolutionX,SystemParameters::ResolutionY };
		Direct3D::GetFont()->DrawText(NULL, g_DebugText[i], -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}
