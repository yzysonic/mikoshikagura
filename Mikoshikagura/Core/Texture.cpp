#include "Texture.h"
#include "Common.h"
#include "Window.h"

Texture* const Texture::none = new Texture;
std::unordered_map<std::string, std::unique_ptr<Texture>> Texture::texture_list;

void Texture::Init(void)
{

	none->pDXTex = nullptr;
	none->name = "none";
	none->divideX = 1;
	none->divideY = 1;
	none->size = Vector2(100.0f, 100.0f);

	texture_list["none"].reset(none);
}

void Texture::Uninit(void)
{
	texture_list["none"].reset(nullptr);
	texture_list.erase("none");
	texture_list.clear();
}

Texture * Texture::Get(std::string name)
{
	try
	{
		return texture_list.at(name).get();
	}
	catch (std::out_of_range&)
	{
		return none;
	}
}

bool Texture::LoadTexture(std::string name, std::string file_name, int divX, int divY)
{

	if (texture_list[name])
		return true;

	char file_dir[256];

	if (file_name == "")
		file_name = name + ".png";
	strcat(strcpy(file_dir, TextureDir), file_name.c_str());

	Texture *texture = new Texture;

	//�e�N�X�`���ڍ׏��擾
	D3DXIMAGE_INFO info;
	if (D3DXGetImageInfoFromFile(file_dir, &info) == D3D_OK)
	{
		//�e�N�X�`���Ǎ�
		D3DXCreateTextureFromFileEx(
			Direct3D::GetDevice(),
			file_dir,
			info.Width, info.Height,

			D3DX_DEFAULT, 0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0x00000000,
			NULL, NULL,
			&texture->pDXTex);
	}
	else
	{
		TCHAR s[128];
		wsprintf(s, _T("�e�N�X�`���[�u%s�v�̓Ǎ��Ɏ��s���܂����B"), file_name.c_str());

#ifndef _DEBUG
		MessageBox(Window::GetHWnd(), s, _T("�G���["), MB_OK | MB_ICONWARNING);
#else
		OutputDebugString(s);
#endif
		texture->pDXTex = nullptr;
		delete texture;

		return false;

	}

	texture->name = name;
	texture->file_name = file_name;
	texture->size.x = (float)info.Width / divX;
	texture->size.y = (float)info.Height / divY;
	texture->divideX = divX;
	texture->divideY = divY;

	texture_list[name].reset(texture);

	return true;
}

void Texture::ReleaseTexture(std::string name)
{
	auto& tex = texture_list[name];
	
	if (!tex)
		return;

	tex->Release();
}

void Texture::MakeTexture(std::string name, int width, int height)
{
	if (texture_list[name])
		return;

	Texture *texture = new Texture;

	// �e�N�X�`���̍쐬
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
		return;
	}

	texture->name = name;
	texture->file_name = "none";
	texture->size.x = (float)width;
	texture->size.y = (float)height;
	texture->divideX = 1;
	texture->divideY = 1;

	texture_list[name].reset(texture);

}

void Texture::Release(void)
{
	SafeRelease(this->pDXTex);
	texture_list[this->name].reset();
}

Texture::~Texture(void)
{
	SafeRelease(this->pDXTex);
}
