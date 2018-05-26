//=============================================================================
//
// モデル処理 [SkinnedModel.cpp]
// Author : 楊子毅
//
//=============================================================================
#include "SkinnedModel.h"
#include "Texture.h"
#include "Direct3D.h"
#include "Object.h"
#include "Game.h"
#include "Window.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PATH		"Data/Model/"	// 読み込むモデル名


//=============================================================================
// 初期化処理
//=============================================================================
SkinnedModel::SkinnedModel(std::string model_name) : Drawable(Layer::DEFAULT, "default")
{
	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();
	AllocateHierarchy	allocater;
	std::string path;


	// モデル関係の初期化
	this->rootFrame = NULL;
	this->meshCont = NULL;
	this->combs = NULL;
	this->boneMap = NULL;
	this->animationSet = NULL;
	this->activeAnimation = 0;
	this->alphaTestEnable = false;
	this->anime_set_num = 0;
	this->anime_speed_scale = 1.0f;

	path = MODEL_PATH + model_name + ".x";

	// Xファイルの読み込み
	HRESULT hr;

	hr = D3DXLoadMeshHierarchyFromX(
		path.c_str(),
		D3DXMESH_SYSTEMMEM, //D3DXMESH_MANAGED
		pDevice,
		&allocater,
		0,
		&this->rootFrame,
		&this->animator);

	if (FAILED(hr))
	{
		TCHAR s[128];
#ifdef _DEBUG
		wsprintf(s, _T("モデル「%s」の読込に失敗しました。\n%s"), model_name.c_str(), Direct3D::GetErrorMesg(hr));
		MessageBox(Window::GetHWnd(), s, _T("エラー"), MB_OK | MB_ICONWARNING);
#endif
		Game::Stop();
		return;
	}

	this->meshCont = FindMeshContainer(this->rootFrame);

	// ボーン設定
	if (this->meshCont->pSkinInfo != NULL)
	{
		this->combs = (D3DXBONECOMBINATION*)this->meshCont->boneCombinationTable->GetBufferPointer();
		this->boneMap = (BoneFrame**)malloc(sizeof(BoneFrame*)*this->meshCont->pSkinInfo->GetNumBones());
		for (DWORD i = 0; i < this->meshCont->pSkinInfo->GetNumBones(); i++)
		{
			this->boneMap[i] = (BoneFrame*)FindFrameByName(this->meshCont->pSkinInfo->GetBoneName(i), this->rootFrame);
			this->boneMap[i]->mtxOffset = *this->meshCont->pSkinInfo->GetBoneOffsetMatrix(i);
		}
	}

	//マテリアルの設定
	for (int i = 0; i < (int)this->meshCont->NumMaterials; i++)
		this->meshCont->pMaterials[i].MatD3D.Ambient = this->meshCont->pMaterials[i].MatD3D.Diffuse;

	// アニメーターの設定
	if (this->animator)
	{
		this->anime_set_num = this->animator->GetMaxNumAnimationSets();
		if (this->anime_set_num > 1)
		{
			int n = this->animator->GetMaxNumAnimationSets();
			this->animationSet = (LPD3DXANIMATIONSET*)malloc(sizeof(LPD3DXANIMATIONSET)*n);
			for (int i = 0; i < n; i++)
				this->animator->GetAnimationSet(i, &this->animationSet[i]);
		}
	}

	// ワールドマトリクスの初期化
	this->mtx_local = /*this->rootFrame->pFrameFirstChild->TransformationMatrix **/ this->rootFrame->TransformationMatrix;
}

//=============================================================================
// 終了処理
//=============================================================================
SkinnedModel::~SkinnedModel(void)
{
	AllocateHierarchy	allocater;

	SafeFree(this->boneMap);
	SafeFree(this->animationSet);

	allocater.DestroyFrame(this->rootFrame);
}


//=============================================================================
// 描画処理
//=============================================================================
void SkinnedModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();
	D3DMATERIAL9 matDef;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, this->alphaTestEnable);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// アニメーション更新
	this->animator->AdvanceTime(0.016f*this->anime_speed_scale, 0);
	// ボーンマトリクス更新
	this->object->transform.UpdateWorldMatrix();
	// 頂点を ローカル空間 → ボーン空間 → フレームローカル空間 → ワールド空間 の順番で変換するためのマトリクス
	((BoneFrame*)this->rootFrame->pFrameFirstChild->pFrameFirstChild)->UpdateMatrix(this->rootFrame->pFrameFirstChild->TransformationMatrix*this->mtx_local*this->object->transform.mtx_world);

	for (DWORD i = 0; i < this->meshCont->numBoneCombinations; i++) {
		DWORD boneNum = 0;
		for (DWORD j = 0; j < this->meshCont->maxFaceInfl; j++) {
			DWORD id = this->combs[i].BoneId[j];
			if (id != UINT_MAX) {
				pDevice->SetTransform(D3DTS_WORLDMATRIX(j), &(this->boneMap[id]->mtxCombined));
				boneNum++;
			}
		}
		pDevice->SetRenderState(D3DRS_VERTEXBLEND, boneNum - 1);

		int attribID = this->combs[i].AttribId;

		//マテリアルの設定
		pDevice->SetMaterial(&this->meshCont->pMaterials[attribID].MatD3D);

		//テクスチャの設定
		if (this->meshCont->pTextures[attribID])
			pDevice->SetTexture(0, this->meshCont->pTextures[attribID]->pDXTex);
		else
			pDevice->SetTexture(0, NULL);
		//描画
		this->meshCont->MeshData.pMesh->DrawSubset(i);

	}

	 //マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void SkinnedModel::SetAnime(int n)
{
	this->animator->SetTrackAnimationSet(0, this->animationSet[n]);
	this->animator->SetTrackPosition(0, 0);
}

void SkinnedModel::SetAnimeSpeedScale(float value)
{
	this->anime_speed_scale = value;
}

float SkinnedModel::GetAnimePeriod(int n)
{
	if(n<this->anime_set_num)
		return (float)this->animationSet[n]->GetPeriod();
	else 
		return 0.0f;
}


D3DXFRAME * SkinnedModel::FindFrameByName(const char * name, D3DXFRAME * frame)
{
	D3DXFRAME *ptr = frame;

	if (strcmp(ptr->Name, name) == 0)
		return ptr;
	if (frame->pFrameFirstChild)
	{
		ptr = FindFrameByName(name, frame->pFrameFirstChild);
		if (ptr) return ptr;
	}
	if (frame->pFrameSibling)
	{
		ptr = FindFrameByName(name, frame->pFrameSibling);
		if (ptr) return ptr;
	}
	return NULL;
	
}