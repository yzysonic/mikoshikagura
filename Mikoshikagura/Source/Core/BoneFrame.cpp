#include "BoneFrame.h"

void BoneFrame::UpdateMatrix(D3DXMATRIX parentMtx)
{
	D3DXMATRIX mtxBone = this->TransformationMatrix * parentMtx;
	this->mtxCombined = this->mtxOffset * mtxBone;
	if (this->pFrameFirstChild)
	{
		if(this->pFrameFirstChild->Name[0] == 'A')
			((BoneFrame*)this->pFrameFirstChild)->UpdateMatrix(mtxBone);
	}
	if (this->pFrameSibling)
	{
		if (this->pFrameSibling->Name[0] == 'A')
			((BoneFrame*)this->pFrameSibling)->UpdateMatrix(parentMtx);
	}
}