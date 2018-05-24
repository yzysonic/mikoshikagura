#include "Snapshot.h"
#include "FadeScreen.h"

void Snapshot::Init(void)
{
	timer.Reset();
	state = 0;
	if (fileName.empty())
		fileName = "Snapshot";
}

void Snapshot::Update(void)
{
	timer++;

	if (!timer.TimeUp())
		return;

	if (state == 0)
	{
		D3DXSaveSurfaceToFile(("Data/Texture/"+fileName+".png").c_str(), D3DXIFF_PNG, RenderTarget::BackBuffer()->pSurface, NULL, NULL);
		FadeScreen::FadeOut(Color::white, 0.15f);
		timer.Reset(0.15f);
		state++;
	}
	else if (state == 1)
	{
		FadeScreen::FadeIn(Color::white, 0.15f);
		SetActive(false);
	}
}
