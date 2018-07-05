#include "Text.h"

Text::Text(void) : Drawable(Layer::UI_02, "default")
{
	auto pDevice = Direct3D::GetDevice();

	area = { 0, 0, SystemParameters::ResolutionX, SystemParameters::ResolutionY };

	width = 36;
	height = 0;
	format = DT_WORDBREAK;

	D3DXCreateFont(pDevice, width, height, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &Font);
}

Text::~Text(void)
{
	if (Font != NULL)
	{
		Font->Release();
		Font = NULL;
	}
	if (!name.empty())
	{
		RemoveFontResource(std::string(BasePath + name + DefaultExtension).c_str());
	}
}

HRESULT Text::OnLostDevice(void)
{
	SafeRelease(this->Font);
	return S_OK;
}

HRESULT Text::OnResetDevice(void)
{
	auto pDevice = Direct3D::GetDevice();

	if (!name.empty())
		D3DXCreateFont(pDevice, width, height, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, name.c_str(), &Font);
	else
		D3DXCreateFont(pDevice, width, height, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &Font);

	return S_OK;
}

void Text::Draw(void)
{

	Font->DrawText(NULL, text.c_str(), -1, &area, format, color);
}

void Text::SetText(std::string string)
{
	text = string;
}

void Text::AddText(char c)
{
	text += c;
}

void Text::ClearText(void)
{
	text.clear();
}

void Text::LoadFont(std::string filename)
{
	auto pDevice = Direct3D::GetDevice();

	if (Font != NULL)
	{
		Font->Release();
		Font = NULL;
	}

	if (!name.empty())
	{
		RemoveFontResource(name.c_str());
	}
	std::string tmp = BasePath + filename + DefaultExtension;
	if (AddFontResource(tmp.c_str()) != 0)
	{
		name = filename;
		D3DXCreateFont(pDevice, width, height, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, name.c_str(), &Font);
	}
	else
	{
		D3DXCreateFont(pDevice, width, height, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &Font);

		name.clear();
	}
}

void Text::SetSize(UINT x, UINT y)
{
	width = x;
	height = y;

	auto pDevice = Direct3D::GetDevice();

	if (Font != NULL)
	{
		Font->Release();
		Font = NULL;
	}

	if (!name.empty())
	{
		D3DXCreateFont(pDevice, width, height, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, name.c_str(), &Font);
	}
	else
	{
		D3DXCreateFont(pDevice, width, height, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &Font);
	}
}

void Text::SetFormat(UINT fm)
{
	format = fm;
}

