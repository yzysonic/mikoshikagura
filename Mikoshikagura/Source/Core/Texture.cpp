#include "Texture.h"
#include "Common.h"
#include "Window.h"
#include "Game.h"

using namespace std;

Texture *const Texture::none = new Texture("none", true);

Texture* Texture::Make(string name, int width, int height)
{
	if (Get(name))
		return nullptr;

	Texture *texture = new Texture(name);

	// テクスチャの作成
	if (FAILED(Direct3D::GetDevice()->CreateTexture(
		width, height,
		1, 0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&texture->pDXTex,
		NULL
	)))
	{
		delete texture;
		return nullptr;
	}

	texture->size.x = (float)width;
	texture->size.y = (float)height;

	InsertToMap(texture);

	return texture;
}

Texture * Texture::InternalLoad(string name, string ext)
{
	auto fullpath = GetFullPath(name + ext);
	auto texture = new Texture(name);
	D3DXIMAGE_INFO info;

	//テクスチャ詳細情報取得
	while (true)
	{
		auto hr = D3DXGetImageInfoFromFile(fullpath.c_str(), &info);
		if (SUCCEEDED(hr))
		{
			//テクスチャ読込
			hr = D3DXCreateTextureFromFileEx(
				Direct3D::GetDevice(),
				fullpath.c_str(),
				info.Width, info.Height,

				D3DX_DEFAULT, 0,
				D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0x00000000,
				NULL, NULL,
				&texture->pDXTex);

			if (SUCCEEDED(hr))
			{
				break;
			}

		}

		// エラー処理
#ifdef _DEBUG
		auto d3dMesg = Direct3D::GetErrorMesg(hr);
#else
		auto d3dMesg = "";
#endif
		switch (ShowErrorMessage(name.c_str(), d3dMesg))
		{
		case IDABORT:
			Game::Stop();
		case IDIGNORE:
			texture->pDXTex = nullptr;
			delete texture;
			return nullptr;
		}

		
	}

	texture->size.x = texture->raw_size.x = (float)info.Width;
	texture->size.y = texture->raw_size.y = (float)info.Height;

	texture->divideX = 1;
	texture->divideY = 1;

	return texture;
}

Texture::Texture(string name) : Resource<Texture>(name)
{
	this->pDXTex = nullptr;
	this->divideX = 1;
	this->divideY = 1;
	this->size = this->raw_size = Vector2(100.0f, 100.0f);
}

Texture::~Texture(void)
{
	SafeRelease(this->pDXTex);
}

void Texture::SetDivision(int x, int y)
{
	this->size.x = this->raw_size.x / x;
	this->size.y = this->raw_size.y / y;
	this->divideX = x;
	this->divideY = y;

}

Texture::Texture(std::string name, bool insertToMap) : Texture(name)
{
	InsertToMap(this);
}
