//=============================================================================
//
// ÉÇÉfÉãèàóù [SkinnedModel.h]
// Author : ókéqãB
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
public:
	D3DXFRAME			*rootFrame;
	XMESHCONTAINER		*meshCont;
	D3DXBONECOMBINATION *combs;
	BoneFrame			**boneMap;
	ID3DXAnimationController *animator;
	LPD3DXANIMATIONSET	*animationSet;
	D3DXMATRIX			mtx_local;
	int					activeAnimation;
	bool				alphaTestEnable;
	int					root_bone_id;

	SkinnedModel(std::string model_name);
	~SkinnedModel(void);
	void Draw(void) override;
	void SetAnime(int n);
	void SetAnimeSpeedScale(float value);
	float GetAnimePeriod(int n);
	D3DXFRAME *FindFrameByName(const char * name);

private:
	static D3DXFRAME *FindFrameByName(const char* name, D3DXFRAME *frame);
	int anime_set_num;
	float anime_speed_scale;
};
