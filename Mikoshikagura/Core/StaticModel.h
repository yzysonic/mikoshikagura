//=============================================================================
//
// モデル処理 [StaticModel.h]
// Author : 楊子毅
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
	Texture				**pTextures;	// テクスチャへのポインタ
	LPD3DXMESH			pMesh;			// ID3DXMeshインターフェイスへのポインタ
	LPD3DXBUFFER		pBuffMaterial;	// マテリアルバッファーへのポインタ
	LPD3DXMATERIAL		pMaterials;		// マテリアル情報へのポインタ
	DWORD				numMaterial;	// マテリアル情報の数
	bool				alphaTestEnable;

	StaticModel(std::string model_name, Layer layer = Layer::DEFAULT);
	~StaticModel(void);
	void Draw(void) override;
	//void DrawModelWireFrame(StaticModel* _this);

};



