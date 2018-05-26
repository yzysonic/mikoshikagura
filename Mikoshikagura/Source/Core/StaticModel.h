//=============================================================================
//
// ���f������ [StaticModel.h]
// Author : �k�q�B
//
//=============================================================================
#pragma once

#include "Common.h"
#include "Drawable.h"
#include "ModelData.h"

class StaticModel : public Drawable
{
public:
	ModelData*			pData;
	bool				alphaTestEnable;

	StaticModel(std::string model_name, Layer layer = Layer::DEFAULT);
	void Draw(void) override;
	//void DrawModelWireFrame(StaticModel* _this);

};



