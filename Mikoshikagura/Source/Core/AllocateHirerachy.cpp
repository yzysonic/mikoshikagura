#include "AllocateHirerachy.h"
#include "Common.h"

STDMETHODIMP AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME *newFrame = NULL;

	if (Name != NULL)
	{
		if (strstr(Name, "Armature") != NULL)
			newFrame = new BoneFrame;
		else
			newFrame = new D3DXFRAME;
	}
	else
		newFrame = new D3DXFRAME;

	newFrame->Name = newStr(Name);
	newFrame->pFrameFirstChild = 0;
	newFrame->pFrameSibling = 0;
	newFrame->pMeshContainer = 0;
	D3DXMatrixIdentity(&newFrame->TransformationMatrix);
	*ppNewFrame = newFrame;

	return D3D_OK;
}

STDMETHODIMP AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	if (pFrameToFree == nullptr)
		return D3D_OK;

	delete[] pFrameToFree->Name;
	delete pFrameToFree;

	return D3D_OK;
}

STDMETHODIMP AllocateHierarchy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	XMESHCONTAINER *newCont = new XMESHCONTAINER;
	
	newCont->Name = newStr(Name);
	newCont->pAdjacency = new DWORD[pMeshData->pMesh->GetNumFaces() * 3];
	memset(newCont->pAdjacency, 0, pMeshData->pMesh->GetNumFaces() * 3 * sizeof(DWORD));
	newCont->MeshData.Type = pMeshData->Type;

	newCont->pSkinInfo = pSkinInfo;
	if (pSkinInfo)
	{
		pSkinInfo->ConvertToBlendedMesh(
			pMeshData->pMesh, 0, pAdjacency, newCont->pAdjacency, 0, 0, &newCont->maxFaceInfl,
			&newCont->numBoneCombinations, &newCont->boneCombinationTable, &newCont->MeshData.pMesh
		);

		pSkinInfo->AddRef();
	}
	else
	{
		newCont->MeshData.pMesh = pMeshData->pMesh;
		newCont->MeshData.pMesh->AddRef();
	}

	newCont->NumMaterials = NumMaterials;
	
	newCont->pMaterials = new D3DXMATERIAL[NumMaterials];
	newCont->pTextures = new Texture*[NumMaterials];
	memcpy(newCont->pMaterials, pMaterials, NumMaterials * sizeof(D3DXMATERIAL));
	for (int i = 0; i < (int)NumMaterials; i++)
	{
		char* fileName = newStr(pMaterials[i].pTextureFilename);
		char* texName = newStr(fileName);
		strtok(texName, ".");

		newCont->pMaterials[i].pTextureFilename = fileName;
		newCont->pTextures[i] = Texture::Get(texName);

		free(texName);
	}

	newCont->pEffects = 0;
	if (pEffectInstances) {
		newCont->pEffects = new D3DXEFFECTINSTANCE;
		newCont->pEffects->pEffectFilename = newStr(pEffectInstances->pEffectFilename);
		newCont->pEffects->NumDefaults = pEffectInstances->NumDefaults;
		newCont->pEffects->pDefaults = new D3DXEFFECTDEFAULT[pEffectInstances->NumDefaults];
		for (DWORD i = 0; i < pEffectInstances->NumDefaults; i++) {
			D3DXEFFECTDEFAULT *src = pEffectInstances->pDefaults + i;
			D3DXEFFECTDEFAULT *dest = newCont->pEffects->pDefaults + i;
			dest->NumBytes = src->NumBytes;
			dest->Type = src->Type;
			dest->pParamName = newStr(src->pParamName);
			dest->pValue = new char[src->NumBytes];
			memcpy(dest->pValue, src->pValue, src->NumBytes);
		}
	}

	*ppNewMeshContainer = newCont;

	return D3D_OK;
}

STDMETHODIMP AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	XMESHCONTAINER *m = (XMESHCONTAINER*)pMeshContainerToFree;

	m->MeshData.pMesh->Release();
	delete[] m->Name;
	delete[] m->pAdjacency;
	if (m->pEffects) {
		for (DWORD i = 0; i < m->pEffects->NumDefaults; i++) {
			D3DXEFFECTDEFAULT *d = m->pEffects->pDefaults + i;
			delete[] d->pParamName;
			delete[] d->pValue;
		}
		delete[] m->pEffects->pDefaults;
		delete[] m->pEffects->pEffectFilename;
		delete m->pEffects;
	}
	for (int i = 0; i < (int)m->NumMaterials; i++)
		delete[] m->pMaterials[i].pTextureFilename;
	delete[] m->pMaterials;
	delete[] m->pTextures;

	if (m->pSkinInfo)
		m->pSkinInfo->Release();

	if (m->boneCombinationTable)
		m->boneCombinationTable->Release();

	delete m;

	return D3D_OK;
}

XMESHCONTAINER * FindMeshContainer(D3DXFRAME * frame)
{
	if (frame->pMeshContainer)
		return (XMESHCONTAINER*)frame->pMeshContainer;

	XMESHCONTAINER* temp = NULL;

	if (frame->pFrameSibling)
	{
		temp = FindMeshContainer(frame->pFrameSibling);
		if (temp) return temp;
	}

	if (frame->pFrameFirstChild)
	{
		temp = FindMeshContainer(frame->pFrameFirstChild);
		if (temp) return temp;
	}


	return NULL;
}

char * AllocateHierarchy::newStr(const char * str)
{
	if(str == NULL)
		return NULL;
	
	char *s = new char[strlen(str) + 1];
	strcpy(s, str);

	return s;
}



