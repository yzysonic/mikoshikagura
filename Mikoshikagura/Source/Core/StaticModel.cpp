//=============================================================================
//
// モデル処理 [StaticModel.cpp]
// Author : 楊子毅
//
//=============================================================================
#include "StaticModel.h"
#include "Direct3D.h"
#include "Object.h"

//=============================================================================
// 初期化処理
//=============================================================================
StaticModel::StaticModel(std::string model_name, Layer layer) : Drawable(layer, "default")
{
	this->alphaTestEnable = false;
	this->pData = ModelData::Get(model_name);
	if (!this->pData)
		active_on_load = false;
}


//=============================================================================
// 描画処理
//=============================================================================
void StaticModel::Draw(void)
{
	auto pDevice = Direct3D::GetDevice();
	D3DMATERIAL9 matDef;
	DWORD prevAlphaTest;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	pDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &prevAlphaTest);

	if (this->alphaTestEnable)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, this->alphaTestEnable);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}

	// ワールドマトリクスの設定
	this->object->transform.UpdateWorldMatrix();
	pDevice->SetTransform(D3DTS_WORLD, &this->object->transform.mtx_world);

	for (DWORD i = 0; i < this->pData->numMaterial; i++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&this->pData->pMaterials[i].MatD3D);

		//テクスチャの設定
		if(this->pData->pTextures[i])
			pDevice->SetTexture(0, this->pData->pTextures[i]->pDXTex);
		else
			pDevice->SetTexture(0, NULL);

		//描画
		this->pData->pMesh->DrawSubset(i);
	}

	//マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, prevAlphaTest);

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
//	// ワールドマトリクスの設定
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
