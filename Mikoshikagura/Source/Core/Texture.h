#pragma once
#include "Direct3D.h"
#include "Vector.h"
#include <unordered_map>
#include <memory>
#include <string>

constexpr const char* TextureDir = "Data/Texture/";

class Texture
{
public:
	static void Init(void);
	static void Uninit(void);
	static Texture* Get(std::string name);
	static bool LoadTexture(std::string name, std::string file_name = "", int divX = 1, int divY = 1);
	static void ReleaseTexture(std::string name);
	static void MakeTexture(std::string name, int width, int height);

	static Texture* const none;

public:
	std::string name;
	std::string file_name;
	LPDIRECT3DTEXTURE9 pDXTex;	// �e�N�X�`���|�C���^
	Vector2 size;				// �e�N�X�`���T�C�Y
	int divideX;				// �e�N�X�`����X������
	int divideY;				// �e�N�X�`����Y������

	void Release(void);
	~Texture(void);

private:
	static std::unordered_map<std::string, std::unique_ptr<Texture>> texture_list;
};