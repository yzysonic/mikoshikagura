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
	LPDIRECT3DTEXTURE9 pDXTex;	// テクスチャポインタ
	Vector2 size;				// テクスチャサイズ
	Vector2 raw_size;			// テクスチャ画像のサイズ
	int divideX;				// テクスチャ内X分割数
	int divideY;				// テクスチャ内Y分割数

	Texture(std::string name);
	~Texture(void);
	void SetDivision(int x, int y);

private:
	Texture(std::string name, bool insertToMap);
};