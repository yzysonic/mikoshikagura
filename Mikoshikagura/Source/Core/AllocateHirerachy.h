#pragma once

#include <d3dx9.h>
#include "Texture.h"
#include "BoneFrame.h"

struct XMESHCONTAINER : public D3DXMESHCONTAINER {
	DWORD maxFaceInfl;
	DWORD numBoneCombinations;
	ID3DXBuffer *boneCombinationTable	= NULL;
	D3DXBONECOMBINATION *combs			= NULL;
	BoneFrame			**boneMap		= NULL;
	Texture **pTextures					= NULL;
	XMESHCONTAINER() : maxFaceInfl(1), numBoneCombinations(0), boneCombinationTable(NULL) {}
};

class AllocateHierarchy : public ID3DXAllocateHierarchy {
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer
		);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
	char *newStr(const char* str);
};

XMESHCONTAINER *FindMeshContainer(D3DXFRAME *frame);