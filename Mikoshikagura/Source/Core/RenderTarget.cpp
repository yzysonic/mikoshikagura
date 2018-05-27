#include "RenderTarget.h"
#include "Common.h"

RenderTarget *const RenderTarget::back_buffer = new RenderTarget("BackBuffer");
//std::vector<RenderTarget*> RenderTarget::render_target_list;

RenderTarget::RenderTarget(std::string name) : Texture(name)
{
	this->pSurface = nullptr;
	this->pDepthSurface = nullptr;
	this->index = -1;
}

RenderTarget::~RenderTarget(void)
{
	//if (this->index >= 0)
	//{
	//	render_target_list[this->index] = render_target_list.back();
	//	render_target_list[this->index]->index = this->index;
	//	render_target_list.pop_back();
	//}
	SafeRelease(pDXTex);
	SafeRelease(pSurface);
	SafeRelease(pDepthSurface);
}

HRESULT RenderTarget::Create(void)
{
	auto pDevice = Direct3D::GetDevice();

	// テクスチャの作成
	if (FAILED(pDevice->CreateTexture(
		(UINT)size.x, (UINT)size.y,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&pDXTex,
		NULL
	)))
	{
		return E_FAIL;
	}

	// テクスチャサーフェースの取得
	if (FAILED(pDXTex->GetSurfaceLevel(0, &pSurface)))
		return E_FAIL;

	// 深度バッファの作成
	if (FAILED(pDevice->CreateDepthStencilSurface(
		(UINT)size.x, (UINT)size.y,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&pDepthSurface,
		NULL
	)))
	{
		return E_FAIL;
	}

	return S_OK;
}

RenderTarget* RenderTarget::BackBuffer(void)
{
	if (back_buffer->pSurface == nullptr)
	{
		auto pDevice = Direct3D::GetDevice();

		LPDIRECT3DSURFACE9 pSurface;
		LPDIRECT3DSURFACE9 pDepthSurface;

		if (FAILED(pDevice->GetRenderTarget(0, &pSurface)))
			return nullptr;
		if (FAILED(pDevice->GetDepthStencilSurface(&pDepthSurface)))
			return nullptr;

		back_buffer->pSurface = pSurface;
		back_buffer->pDepthSurface = pDepthSurface;
		back_buffer->size = back_buffer->raw_size = Vector2((float)SystemParameters::ResolutionX, (float)SystemParameters::ResolutionY);

	}
	
	return back_buffer;
}

RenderTarget * RenderTarget::Make(std::string name, int width, int height)
{
	auto pDevice = Direct3D::GetDevice();
	auto renderTarget = new RenderTarget(name);

	renderTarget->size = renderTarget->raw_size = Vector2((float)width, (float)height);

	if (FAILED(renderTarget->Create()))
	{
		delete renderTarget;
		return nullptr;
	}

	//renderTarget->index = render_target_list.size();
	//render_target_list.push_back(renderTarget);
	InsertToMap(renderTarget);

	return renderTarget;
}

HRESULT RenderTarget::OnLostDevice(void)
{

	SafeRelease(this->pDXTex);
	SafeRelease(this->pSurface);
	SafeRelease(this->pDepthSurface);

	return S_OK;
}

HRESULT RenderTarget::OnResetDevice(void)
{
	if (this == this->back_buffer)
	{
		BackBuffer();
		return S_OK;
	}

	return this->Create();
}
