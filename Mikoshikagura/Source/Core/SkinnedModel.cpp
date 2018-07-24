//=============================================================================
//
// ���f������ [SkinnedModel.cpp]
// Author : �k�q�B
//
//=============================================================================
#include "SkinnedModel.h"
#include "Texture.h"
#include "Direct3D.h"
#include "Object.h"
#include "Game.h"
#include "Window.h"
#include "Time.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PATH		"Data/Model/"	// �ǂݍ��ރ��f����


//=============================================================================
// ����������
//=============================================================================
SkinnedModel::SkinnedModel(std::string model_name) : Drawable(Layer::DEFAULT, "default")
{
	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();
	AllocateHierarchy	allocater;
	std::string path;


	// ���f���֌W�̏�����
	this->rootFrame = NULL;
	this->animationSet = NULL;
	this->activeAnimation = 0;
	this->alphaTestEnable = false;
	this->anim_set_num = 0;
	this->anim_speed_scale = 1.0f;

	path = MODEL_PATH + model_name + ".x";

	// X�t�@�C���̓ǂݍ���
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
		wsprintf(s, _T("���f���u%s�v�̓Ǎ��Ɏ��s���܂����B\n%s"), model_name.c_str(), Direct3D::GetErrorMesg(hr));
		MessageBox(Window::GetHWnd(), s, _T("�G���["), MB_OK | MB_ICONWARNING);
#endif
		Game::Stop();
		return;
	}

	InitMeshContainer(this->rootFrame);

	// �A�j���[�^�[�̐ݒ�
	if (this->animator)
	{
		this->anim_set_num = this->animator->GetMaxNumAnimationSets();
		if (this->anim_set_num > 1)
		{
			int n = this->animator->GetMaxNumAnimationSets();
			this->animationSet = (LPD3DXANIMATIONSET*)malloc(sizeof(LPD3DXANIMATIONSET)*n);
			for (int i = 0; i < n; i++)
				this->animator->GetAnimationSet(i, &this->animationSet[i]);
		}
	}

	// ���[���h�}�g���N�X�̏�����
	this->mtx_local = /*this->rootFrame->pFrameFirstChild->TransformationMatrix **/ this->rootFrame->TransformationMatrix;
}

//=============================================================================
// �I������
//=============================================================================
SkinnedModel::~SkinnedModel(void)
{
	AllocateHierarchy	allocater;
	for (auto mesh : this->meshContList)
	{
		SafeDelete(mesh->boneMap);

	}
	SafeFree(this->animationSet);
	D3DXFrameDestroy(this->rootFrame, &allocater);
}


//=============================================================================
// �`�揈��
//=============================================================================
void SkinnedModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, this->alphaTestEnable);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// �A�j���[�V�����X�V
	AdvanceTime(Time::DeltaTime()*this->anim_speed_scale);
	// �{�[���}�g���N�X�X�V
	this->object->transform.UpdateWorldMatrix();
	// ���_�� ���[�J����� �� �{�[����� �� �t���[�����[�J����� �� ���[���h��� �̏��Ԃŕϊ����邽�߂̃}�g���N�X
	((BoneFrame*)this->rootFrame->pFrameFirstChild->pFrameFirstChild)->UpdateMatrix(this->rootFrame->pFrameFirstChild->TransformationMatrix*this->mtx_local*this->object->transform.mtx_world);

	for (auto mesh : this->meshContList)
	{
		for (DWORD i = 0; i < mesh->numBoneCombinations; i++) {
			DWORD boneNum = 0;
			for (DWORD j = 0; j < mesh->maxFaceInfl; j++) {
				DWORD id = mesh->combs[i].BoneId[j];
				if (id != UINT_MAX) {
					pDevice->SetTransform(D3DTS_WORLDMATRIX(j), &(mesh->boneMap[id]->mtxCombined));
					boneNum++;
				}
			}
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, boneNum - 1);

			int attribID = mesh->combs[i].AttribId;

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&mesh->pMaterials[attribID].MatD3D);

			//�e�N�X�`���̐ݒ�
			if (mesh->pTextures[attribID])
				pDevice->SetTexture(0, mesh->pTextures[attribID]->pDXTex);
			else
				pDevice->SetTexture(0, NULL);
			//�`��
			mesh->MeshData.pMesh->DrawSubset(i);

		}

	}


	 //�}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void SkinnedModel::SetAnimation(int n)
{
	this->animator->SetTrackAnimationSet(0, this->animationSet[n]);
	this->animator->SetTrackPosition(0, 0);
}

void SkinnedModel::SetAnimation(std::string name)
{
	LPD3DXANIMATIONSET animationSet;
	this->animator->GetAnimationSetByName(name.c_str(), &animationSet);

	if (this->GetCurrentAnimation() == animationSet)
		return;

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;
	this->animator->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs
	this->animator->SetTrackAnimationSet(1, this->GetCurrentAnimation());
	this->animator->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	this->animator->SetTrackAnimationSet(0, animationSet);

	// �g���b�N�̍���
	this->animator->SetTrackEnable(0, true);
	this->animator->SetTrackEnable(1, true);

	this->animator->SetTrackPosition(0, 0);
	this->anim_shift_time = 0.15f;
	this->anim_timer = 0.0f;
}

void SkinnedModel::SetAnimationSpeedScale(float value)
{
	this->anim_speed_scale = value;
}

LPD3DXANIMATIONSET SkinnedModel::GetAnimation(std::string name)
{
	LPD3DXANIMATIONSET animationSet;
	this->animator->GetAnimationSetByName(name.c_str(), &animationSet);
	return animationSet;
}

LPD3DXANIMATIONSET SkinnedModel::GetCurrentAnimation(void)
{
	LPD3DXANIMATIONSET animationSet;
	this->animator->GetTrackAnimationSet(0, &animationSet);
	return animationSet;
}

float SkinnedModel::GetAnimationPeriod(int n)
{
	if(n<this->anim_set_num)
		return (float)this->animationSet[n]->GetPeriod();
	else 
		return 0.0f;
}

float SkinnedModel::GetAnimationPeriod(std::string name)
{
	LPD3DXANIMATIONSET animationSet;
	this->animator->GetAnimationSetByName(name.c_str(), &animationSet);
	if (animationSet)
		return animationSet->GetPeriod();
	else
		return 0.0f;
}

D3DXFRAME * SkinnedModel::FindFrameByName(const char * name)
{
	return FindFrameByName(name, rootFrame);
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

void SkinnedModel::InitMeshContainer(LPD3DXFRAME frame)
{
	if (frame->pMeshContainer)
	{
		auto mesh = (XMESHCONTAINER*)frame->pMeshContainer;

		// �{�[���ݒ�
		if (mesh->pSkinInfo != NULL)
		{
			mesh->combs = (D3DXBONECOMBINATION*)mesh->boneCombinationTable->GetBufferPointer();
			mesh->boneMap = new BoneFrame*[mesh->pSkinInfo->GetNumBones()];
			for (DWORD i = 0; i < mesh->pSkinInfo->GetNumBones(); i++)
			{
				mesh->boneMap[i] = (BoneFrame*)FindFrameByName(mesh->pSkinInfo->GetBoneName(i), this->rootFrame);
				mesh->boneMap[i]->mtxOffset = *mesh->pSkinInfo->GetBoneOffsetMatrix(i);
			}
		}

		//�}�e���A���̐ݒ�
		for (int i = 0; i < (int)mesh->NumMaterials; i++)
			mesh->pMaterials[i].MatD3D.Ambient = mesh->pMaterials[i].MatD3D.Diffuse;

		this->meshContList.emplace_back(mesh);
	}

	if (frame->pFrameSibling)
	{
		InitMeshContainer(frame->pFrameSibling);
	}

	if (frame->pFrameFirstChild)
	{
		InitMeshContainer(frame->pFrameFirstChild);
	}

}

void SkinnedModel::AdvanceTime(float time)
{
	this->anim_timer += time;

	// ���[�V�����u�����h
	if (this->anim_timer < this->anim_shift_time)
	{
		auto weight = this->anim_timer / this->anim_shift_time;
		
		this->animator->SetTrackWeight(0, weight);
		this->animator->SetTrackWeight(1, 1 - weight);
	}
	else
	{
		this->animator->SetTrackWeight(0, 1.0f);
		this->animator->SetTrackEnable(1, false);
	}

	this->animator->AdvanceTime(time, &callback_handler);
}
