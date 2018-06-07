#include "Text.h"

Text::Text(void) : Drawable(Layer::UI_02, "default")
{
	auto pDevice = Direct3D::GetDevice();

	area = { 0, 0, SystemParameters::ResolutionX, SystemParameters::ResolutionY };

	D3DXCreateFont(pDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &Font);
}

Text::~Text(void)
{
	if (Font != NULL)
	{
		Font->Release();
		Font = NULL;
	}
	if (!path.empty())
	{
		RemoveFontResource(path.c_str());
	}
}

void Text::Draw(void)
{

	Font->DrawText(NULL, text.c_str(), -1, &area, DT_WORDBREAK, Color::white);
}

void Text::SetText(std::string string)
{
	text = string;
}

void Text::AddText(std::string string)
{
	text += string;
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

	if (!path.empty())
	{
		RemoveFontResource(path.c_str());
	}

	if (AddFontResource(filename.c_str()) != 0)
	{
		path = filename;

		D3DXCreateFont(pDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "APJapanesefont", &Font);
	}
	else
	{
		D3DXCreateFont(pDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &Font);

		path.clear();
	}
}

