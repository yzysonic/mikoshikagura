//=============================================================================
//
// �t�@�C�����Fgraphics.cpp
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#include "Direct3D.h"
#include "Common.h"

#ifdef _DEBUG
#include <DxErr.h>
#endif

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#ifdef _DEBUG
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "legacy_stdio_definitions.lib")
#endif

LPDIRECT3D9				Direct3D::s_pD3D = NULL;
LPDIRECT3DDEVICE9		Direct3D::s_pDevice = NULL;
LPD3DXFONT				Direct3D::s_pFont = NULL;
D3DPRESENT_PARAMETERS	Direct3D::s_d3dpp = {};
HWND					Direct3D::s_hWnd = NULL;
std::vector<ILostAndReset*> Direct3D::s_reset_list;


//=============================================================================
// �O���t�B�b�N�̏���������
//=============================================================================
HRESULT Direct3D::Init(HWND hWnd, bool bWindowMode)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	s_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (s_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SystemParameters::ResolutionX;	// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SystemParameters::ResolutionY;	// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;			// �o�b�N�o�b�t�@�̃t�H�[�}�b�g�͌��ݐݒ肳��Ă�����̂��g��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// �f���M���ɓ������ăt���b�v����
	d3dpp.hDeviceWindow = hWnd;							// �g�p����E�B���h
	d3dpp.Windowed = bWindowMode;						// �E�B���h�E���[�h�̐ݒ�
	d3dpp.EnableAutoDepthStencil = TRUE;				// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.BackBufferFormat = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,								// �f�B�X�v���C�^�C�v
		hWnd,										// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,		// �f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,										// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&s_pDevice)))							// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&s_pDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&s_pDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	s_d3dpp = d3dpp;
	s_hWnd = hWnd;

	D3DXCreateFont(s_pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &s_pFont);

	return S_OK;
}

//=============================================================================
// �O���t�B�b�N�̏I������
//=============================================================================
void Direct3D::Uninit()
{
	SafeRelease(s_pDevice);
	SafeRelease(s_pD3D);
}


LPDIRECT3DDEVICE9 Direct3D::GetDevice()
{
	return s_pDevice;
}

bool Direct3D::ResetDevice()
{

	s_pFont->OnLostDevice();
	for (auto reset : s_reset_list)
		reset->OnLostDevice();

	HRESULT hr = s_pDevice->Reset(&s_d3dpp);

	s_pFont->OnResetDevice();
	for (auto reset : s_reset_list)
		reset->OnResetDevice();

	if (FAILED(hr))
	{
#ifdef _DEBUG
		ShowErrorMesg(hr);
		hr = s_pDevice->TestCooperativeLevel();
		ShowErrorMesg(hr);
#endif
		return false;
	}

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	s_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// �J�����O���s��Ȃ�
	s_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	s_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	s_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	s_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	s_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	s_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	s_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	s_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	s_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���X�e�[�W���Z��������
	s_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	s_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	s_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// �Q�Ԗڂ̃A���t�@����


	return true;
}

LPD3DXFONT Direct3D::GetFont()
{
	return s_pFont;
}

bool Direct3D::SetWindowMode(bool windowMode)
{
	s_d3dpp.Windowed = windowMode;

	// �E�B���h�E���[�h
	if (windowMode)
	{
		s_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;						// �o�b�N�o�b�t�@
		s_d3dpp.FullScreen_RefreshRateInHz = 0;							// ���t���b�V�����[�g
		//s_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	// �t���X�N���[�����[�h
	else
	{
		s_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;						// �o�b�N�o�b�t�@
		s_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
		s_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	return ResetDevice();
}

#ifdef _DEBUG
void Direct3D::ShowErrorMesg(const HRESULT &hr)
{
	MessageBox(s_hWnd, DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK | MB_ICONWARNING);
}
const char* Direct3D::GetErrorMesg(const HRESULT & hr)
{
	return DXGetErrorString(hr);
}
#endif

ILostAndReset::ILostAndReset(void)
{
	Direct3D::s_reset_list.push_back(this);
}

ILostAndReset::~ILostAndReset(void)
{
	auto& list = Direct3D::s_reset_list;
	for (auto it = list.begin(); it != list.end(); it++)
	{
		if (*it == this)
		{
			list.erase(it);
			return;
		}
	}
		
}
