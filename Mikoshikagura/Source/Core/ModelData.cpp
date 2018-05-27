#include "ModelData.h"
#include "Window.h"
#include "Game.h"

using namespace std;

ModelData* ModelData::InternalLoad(string name, std::string ext)
{
	auto fullpath	= GetFullPath(name+ext);
	auto data		= new ModelData(name);

	while(true)
	{
		// X�t�@�C���̓ǂݍ���
		auto hr = D3DXLoadMeshFromX(
			fullpath.c_str(),
			D3DXMESH_SYSTEMMEM,
			Direct3D::GetDevice(),
			NULL,
			&data->pBuffMaterial,
			NULL,
			&data->numMaterial,
			&data->pMesh);

		if (SUCCEEDED(hr))
			break;

		// �G���[����
#ifdef _DEBUG
		auto d3dMesg = Direct3D::GetErrorMesg(hr);
#else
		auto d3dMesg = "";
#endif
		switch (ShowErrorMessage(name.c_str(), d3dMesg))
		{
		case IDABORT:
			Game::Stop();
		case IDIGNORE:
			delete data;
			return nullptr;
		}
	}

	//�}�e���A���ƃe�N�X�`���̐ݒ�
	data->pMaterials = (LPD3DXMATERIAL)data->pBuffMaterial->GetBufferPointer();
	data->pTextures = new Texture*[data->numMaterial];

	for (DWORD i = 0; i < data->numMaterial; i++)
	{
		data->pMaterials[i].MatD3D.Ambient = data->pMaterials[i].MatD3D.Diffuse;

		auto texFile = data->pMaterials[i].pTextureFilename;
		if (texFile)
		{
			char texName[256];
			strcpy(texName, texFile);
			strtok(texName, ".");
			data->pTextures[i] = Texture::Get(texName);
		}
		else
			data->pTextures[i] = nullptr;
	}

	return data;
}

ModelData::ModelData(string name) : Resource<ModelData>(name)
{
	pTextures		= nullptr;
	pBuffMaterial	= nullptr;
	pMesh			= nullptr;
}

ModelData::~ModelData(void)
{
	SafeRelease(this->pMesh);
	SafeRelease(this->pBuffMaterial);
	SafeDelete(this->pTextures);
}
