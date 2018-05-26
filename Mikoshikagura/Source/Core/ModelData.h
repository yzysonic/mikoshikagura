#pragma once
#include "Common.h"
#include "Direct3D.h"
#include "Resource.h"
#include "Texture.h"

class ModelData : public Resource<ModelData>
{
	friend class Resource<ModelData>;
public:
	static constexpr char* BasePath = "Model/";
	static constexpr char* DefaultExtension = ".x";

private:
	static ModelData* InternalLoad(std::string name, std::string ext);

public:
	Texture				**pTextures;	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			pMesh;			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
	LPD3DXMATERIAL		pMaterials;		// �}�e���A�����ւ̃|�C���^
	DWORD				numMaterial;	// �}�e���A�����̐�


	ModelData(std::string name);
	~ModelData(void);
private:
	LPD3DXBUFFER		pBuffMaterial;	// �}�e���A���o�b�t�@�[�ւ̃|�C���^
};