#pragma once

#include <d3dx9.h>

class BoneFrame : public D3DXFRAME
{
public:
	D3DXMATRIX	mtxOffset;
	D3DXMATRIX	mtxCombined;

	void UpdateMatrix(D3DXMATRIX parentMtx);
};