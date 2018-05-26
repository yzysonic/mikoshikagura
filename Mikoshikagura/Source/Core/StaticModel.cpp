//=============================================================================
//
// ���f������ [StaticModel.cpp]
// Author : �k�q�B
//
//=============================================================================
#include "StaticModel.h"
#include "Direct3D.h"
#include "Input.h"
#include "Object.h"
#include "Window.h"
#include "Game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PATH		"Data/Model/"	// �ǂݍ��ރ��f����

//=============================================================================
// ����������
//=============================================================================
StaticModel::StaticModel(std::string model_name, Layer layer) : Drawable(layer, "default")
{
	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();
	std::string path;

	// ���f���֌W�̏�����
	this->pBuffMaterial = NULL;
	this->pMesh = NULL;
	this->pTextures = NULL;
	this->alphaTestEnable = false;

	path = MODEL_PATH + model_name + ".x";

	// X�t�@�C���̓ǂݍ���
	HRESULT hr;
	hr = D3DXLoadMeshFromX(
		path.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&this->pBuffMaterial,
		NULL,
		&this->numMaterial,
		&this->pMesh);

	if (FAILED(hr))
	{
#ifdef _DEBUG
		TCHAR s[128];
		wsprintf(s, _T("���f���u%s�v�̓Ǎ��Ɏ��s���܂����B\n%s"), model_name.c_str(), Direct3D::GetErrorMesg(hr));
		MessageBox(Window::GetHWnd(), s, _T("�G���["), MB_OK | MB_ICONWARNING);
#endif
		Game::Stop();
		return;
	}

	//�}�e���A���ƃe�N�X�`���̐ݒ�
	this->pMaterials = (LPD3DXMATERIAL)this->pBuffMaterial->GetBufferPointer();
	this->pTextures = (Texture**)malloc(sizeof(Texture*)*this->numMaterial);

	for (DWORD i = 0; i < this->numMaterial; i++)
	{
		this->pMaterials[i].MatD3D.Ambient = this->pMaterials[i].MatD3D.Diffuse;

		char* texFile = this->pMaterials[i].pTextureFilename;
		if (texFile)
		{
			char texName[256];
			strcpy(texName, texFile);
			strtok(texName, ".");
			this->pTextures[i] = Texture::Get(texName);
		}
		else
			this->pTextures[i] = NULL;
	}

}


//=============================================================================
// �I������
//=============================================================================
StaticModel::~StaticModel(void)
{
	SafeRelease(this->pMesh);
	SafeRelease(this->pBuffMaterial);
	SafeFree(this->pTextures);
}


//=============================================================================
// �`�揈��
//=============================================================================
void StaticModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, this->alphaTestEnable);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���[���h�}�g���N�X�̐ݒ�
	this->object->transform.UpdateWorldMatrix();
	pDevice->SetTransform(D3DTS_WORLD, &this->object->transform.mtx_world);

	for (DWORD i = 0; i < this->numMaterial; i++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&this->pMaterials[i].MatD3D);

		//�e�N�X�`���̐ݒ�
		if(this->pTextures[i])
			pDevice->SetTexture(0, this->pTextures[i]->pDXTex);
		else
			pDevice->SetTexture(0, NULL);

		//�`��
		this->pMesh->DrawSubset(i);
	}

	//�}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//void DrawModelWireFrame(StaticModel * this)
//{
//	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();
//	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
//	LPDIRECT3DINDEXBUFFER9 pIdxBuff;
//	DWORD lighting_state;
//
//	this->pMesh->GetVertexBuffer(&pVtxBuff);
//	this->pMesh->GetIndexBuffer(&pIdxBuff);
//
//	// ���[���h�}�g���N�X�̐ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &this->transform.mtxWorld);
//
//	pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(64, 219, 111, 200));
//	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
//	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
//
//	pDevice->GetRenderState(D3DRS_LIGHTING, &lighting_state);
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//
//	pDevice->SetStreamSource(0, pVtxBuff, 0, this->pMesh->GetNumBytesPerVertex());
//	pDevice->SetFVF(this->pMesh->GetFVF());
//	pDevice->SetIndices(pIdxBuff);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this->pMesh->GetNumVertices(), 0, this->pMesh->GetNumFaces());
//	
//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//
//	pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
//	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
//	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, lighting_state);
//	
//}
