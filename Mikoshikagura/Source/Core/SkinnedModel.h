//=============================================================================
//
// ���f������ [SkinnedModel.h]
// Author : �k�q�B
//
//=============================================================================
#pragma once

#include "Common.h"
#include "AllocateHirerachy.h"
#include "BoneFrame.h"
#include "Drawable.h"
#include "Transform.h"

class SkinnedModel : public Drawable
{
private:
	class CallbackHandler : public ID3DXAnimationCallbackHandler
	{
	public:
		HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
		{
			if(pCallbackData)
				(*(Action*)pCallbackData)();
			return D3D_OK;
		}
	} callback_handler;

public:
	D3DXFRAME			*rootFrame;
	std::vector<XMESHCONTAINER*> meshContList;
	ID3DXAnimationController *animator;
	LPD3DXANIMATIONSET	*animationSet;
	D3DXMATRIX			mtx_local;
	int					activeAnimation;
	bool				alphaTestEnable;
	int					root_bone_id;

	SkinnedModel(std::string model_name);
	~SkinnedModel(void);
	void Draw(void) override;
	void SetAnimation(int n);
	void SetAnimation(std::string name);
	void SetAnimationSpeedScale(float value);
	LPD3DXANIMATIONSET GetAnimation(std::string name);
	LPD3DXANIMATIONSET GetCurrentAnimation(void);
	float GetAnimationPeriod(int n);
	float GetAnimationPeriod(std::string name);
	D3DXFRAME *FindFrameByName(const char * name);

private:
	int anim_set_num;
	float anim_speed_scale;
	float anim_timer;
	float anim_shift_time;

	static D3DXFRAME *FindFrameByName(const char* name, D3DXFRAME *frame);
	void InitMeshContainer(LPD3DXFRAME frame);
	void AdvanceTime(float time);
};
