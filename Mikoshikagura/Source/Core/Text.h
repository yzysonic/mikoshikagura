#pragma once
#include "Core.h"
#include "Common.h"
#include "Drawable.h"

class Text : public Drawable
{
public:
	Text(void);
	~Text(void);
	void Draw(void) override;
	void SetText(std::string string);
	void AddText(char c);
	void ClearText(void);
	void LoadFont(std::string filename);
	RECT area;

private:
	LPD3DXFONT	Font = NULL;
	std::string text, path;
};