#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include "Common.h"
#include <vector>

class RenderTarget : public Texture, public ILostAndReset
{
public:
	static RenderTarget* BackBuffer(void);
	static RenderTarget* Make(std::string name, int width, int height);

private:
	static RenderTarget *const back_buffer;
	//static std::vector<RenderTarget*> render_target_list;

public:
	LPDIRECT3DSURFACE9 pSurface;
	LPDIRECT3DSURFACE9 pDepthSurface;

	HRESULT OnLostDevice(void) override;
	HRESULT OnResetDevice(void) override;

	RenderTarget(std::string name);
	~RenderTarget(void);

private:
	int index;
	HRESULT Create(void);
};