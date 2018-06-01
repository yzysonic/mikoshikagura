#pragma once

#include "Common.h"
#include "Texture.h"
#include "Drawable.h"
#include "Object.h"
#include "Component.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	RECT_NUM_VERTEX		(4)		// ���_��
#define	RECT_NUM_POLYGON	(2)		// �|���S����

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 3D�|���S�����_�t�H�[�}�b�g( ���_���W / �@�� / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct Vertex2D
{
	Vector3 pos;		// ���_���W
	float rhw;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	Color diffuse;		// ���ˌ�
	Vector2 uv;			// �e�N�X�`�����W
};

struct Vertex3D
{
	Vector3 pos;
	Vector3 nor;
	Color diffuse;
	Vector2 uv;
};

class RectPolygon2D : public Drawable
{
public:
	Vertex2D vertex[RECT_NUM_VERTEX];		// ���_���i�[���[�N
	Texture *pTexture;						// �e�N�X�`���[�ւ̃|�C���^
	float radius;							// ���_�v�Z�p���a
	float baseAngle;						// ���_�v�Z�p�p�x
	
	RectPolygon2D(Texture* texture, Layer layer = Layer::DEFAULT, std::string render_space_name = "default");
	RectPolygon2D(std::string texture_name = "none", Layer layer = Layer::DEFAULT, std::string render_space_name = "default");
	void Draw(void) override;
	void SetColor(Color color);
	void SetSize(float x, float y);
	Vector2 GetSize(void);
	void setPattern(unsigned int pattern);
	void SetTexture(std::string texture_name);
	void SetTexture(Texture* texture);

protected:
	Vector2 size;	// �\������T�C�Y
	int pattern;	// �\������p�^�[��

	void transformVertex(void);
};

class RectPolygon : public Drawable, public ILostAndReset
{
public:
	Texture *pTexture;

	RectPolygon(Texture* texture, Layer layer = Layer::DEFAULT, std::string render_space_name = "default");
	RectPolygon(std::string texture_name = "none", Layer layer = Layer::DEFAULT, std::string render_space_name = "default");
	~RectPolygon(void);
	HRESULT OnLostDevice(void);
	HRESULT OnResetDevice(void);
	void Draw(void) override;
	void SetColor(Color color);
	Vector2 GetSize(void);
	void SetSize(Vector2 size);
	void SetTexture(std::string texture_name);
	void SetTexture(Texture* texture);
	void LockBuff(Vertex3D** pVtx);
	void UnlockBuff(void);

protected:
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	Vector2 size;

	HRESULT InitBuffer(void);

};
