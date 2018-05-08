//=============================================================================
//
// �t�@�C�����Fgraphics.h
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#pragma once

#include <d3dx9.h>
#include <vector>

class ILostAndReset
{
public:
	ILostAndReset(void);
	~ILostAndReset(void);
	virtual HRESULT OnLostDevice(void) = 0;
	virtual HRESULT OnResetDevice(void) = 0;
};


class Direct3D
{
	friend class ILostAndReset;
public:
	static HRESULT Init(HWND hWnd, bool bWindowMode);
	static void Uninit(void);
	static LPDIRECT3DDEVICE9 GetDevice(void);
	static bool ResetDevice(void);
	static LPD3DXFONT GetFont(void);
	static bool SetWindowMode(bool windowMode);
#ifdef _DEBUG
	static void ShowErrorMesg(const HRESULT &hr);
	static const char* GetErrorMesg(const HRESULT &hr);
#endif

private:
	static LPDIRECT3D9				s_pD3D;		// Direct3D�I�u�W�F�N�g
	static LPDIRECT3DDEVICE9		s_pDevice;	// Device�I�u�W�F�N�g(�`��ɕK�v)
	static LPD3DXFONT				s_pFont;	// �t�H���g
	static D3DPRESENT_PARAMETERS	s_d3dpp;	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
	static HWND						s_hWnd;		// �E�B���h�E�n���h��
	static std::vector<ILostAndReset*> s_reset_list;
};


