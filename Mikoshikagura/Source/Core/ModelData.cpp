#include "ModelData.h"
#include "Window.h"
#include "Game.h"

using namespace std;

ModelData* ModelData::_Load(string path)
{

	auto pDevice	= Direct3D::GetDevice();
	auto fullpath	= Resource<ModelData>::BasePath + (BasePath + path + FileExtension);
	auto data		= new ModelData(path);

	while(true)
	{
		// Xファイルの読み込み
		auto hr = D3DXLoadMeshFromX(
			fullpath.c_str(),
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&data->pBuffMaterial,
			NULL,
			&data->numMaterial,
			&data->pMesh);

		if (SUCCEEDED(hr))
			break;

		// エラー処理
		TCHAR s[128];

#ifdef _DEBUG
		wsprintf(s, _T("モデル「%s」の読込に失敗しました。\n%s"), path.c_str(), Direct3D::GetErrorMesg(hr));
#else
		wsprintf(s, _T("モデル「%s」の読込に失敗しました。"), path.c_str());
#endif
			
		switch (ShowErrorMessage(s))
		{
		case IDRETRY:
			break;

		case IDABORT:
			Game::Stop();
		case IDIGNORE:
			delete data;
			return nullptr;
		}
	}

	//マテリアルとテクスチャの設定
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
