#pragma once
#include "Direct3D.h"
#include "Vector.h"
#include "Resource.h"
#include <string>

class Texture : public Resource<Texture>
{
	friend class Resource<Texture>;

public:
	static constexpr char* BasePath = "Texture/";
	static constexpr char* DefaultExtension = ".png";
	static Texture *const none;
	static Texture* Make(std::string name, int width, int height);

private:
	static Texture* InternalLoad(std::string name, std::string ext);

public:
	LPDIRECT3DTEXTURE9 pDXTex;	// �e�N�X�`���|�C���^
	Vector2 size;				// �e�N�X�`���T�C�Y
	Vector2 raw_size;			// �e�N�X�`���摜�̃T�C�Y
	int divideX;				// �e�N�X�`����X������
	int divideY;				// �e�N�X�`����Y������

	Texture(std::string name);
	virtual ~Texture(void);
	void SetDivision(int x, int y);

private:
	Texture(std::string name, bool insertToMap);
};