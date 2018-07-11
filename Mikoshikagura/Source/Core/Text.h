#pragma once
#include "Core.h"
#include "Common.h"
#include "Drawable.h"
#include "Component.h"

class Text : public Drawable, public ILostAndReset
{
public:
	static constexpr char* BasePath = "Data/Font/";
	static constexpr char* DefaultExtension = ".ttf";

public:
	Text(void);
	~Text(void);
	HRESULT OnLostDevice(void) override;
	HRESULT OnResetDevice(void) override;
	void Draw(void) override;
	void SetText(std::string string);
	void AddText(char c);
	void ClearText(void);
	void LoadFont(std::string filename);
	void SetSize(UINT x, UINT y);
	void SetFormat(UINT fm);
	RECT area;

private:
	LPD3DXFONT	Font = NULL;
	std::string text, name;
	UINT width, height, format;
};