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
	Texture				**pTextures;	// テクスチャへのポインタ
	LPD3DXMESH			pMesh;			// ID3DXMeshインターフェイスへのポインタ
	LPD3DXMATERIAL		pMaterials;		// マテリアル情報へのポインタ
	DWORD				numMaterial;	// マテリアル情報の数


	ModelData(std::string name);
	~ModelData(void);
private:
	LPD3DXBUFFER		pBuffMaterial;	// マテリアルバッファーへのポインタ
};