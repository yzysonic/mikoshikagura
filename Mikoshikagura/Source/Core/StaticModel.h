//=============================================================================
//
// ���f������ [StaticModel.h]
// Author : �k�q�B
//
//=============================================================================
#pragma once

#include "Common.h"
#include "Drawable.h"
#include "Transform.h"
#include "Texture.h"

class StaticModel : public Drawable
{
public:
	Texture				**pTextures;	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			pMesh;			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
	LPD3DXBUFFER		pBuffMaterial;	// �}�e���A���o�b�t�@�[�ւ̃|�C���^
	LPD3DXMATERIAL		pMaterials;		// �}�e���A�����ւ̃|�C���^
	DWORD				numMaterial;	// �}�e���A�����̐�
	bool				alphaTestEnable;

	StaticModel(std::string model_name, Layer layer = Layer::DEFAULT);
	~StaticModel(void);
	void Draw(void) override;
	//void DrawModelWireFrame(StaticModel* _this);

};



