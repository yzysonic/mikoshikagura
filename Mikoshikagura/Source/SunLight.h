#pragma once
#include "Core/Core.h"

class SunLightBehavior : public IParticleBehavior
{
public:
	Camera* camera;
	bool overlay;
	float opacity_max;
	float opacity_min;
	float display_interval;

	SunLightBehavior(void)
	{
		overlay = true;
		camera = RenderSpace::Get("default")->GetCamera(0);
		opacity_max = overlay ? 1.0f : 0.4f;
		opacity_min = 0.0f;
		display_interval = 2.5f;

	}

	void Init(ParticleElement & element) override
	{
		element.transform.setRotation(0.0f, 0.0f, Deg2Rad(26.5f));
		element.color.a = 0;
		element.timer.Reset(Randomf(0.3f, 15.0f));
	}

	void Update(ParticleElement & element) override
	{
		element.timer++;

		if (element.color.a > 0)
		{
			auto a = Lerpf(opacity_min, opacity_max, 0.5f*(1.0f - cosf(element.timer.Progress()*2.0f*PI)));
			element.color.a = max(1, (unsigned char)(255 * a));

			element.transform.position.y = camera->transform.position.y + 30.0f;

			if (element.timer.TimeUp())
			{
				element.color.a = 0;
				element.timer.Reset(Randomf(0.3f, 15.0f));
			}

		}
		else
		{
			if (element.timer.TimeUp())
			{
				element.transform.position.x = camera->transform.position.x + (-30.0f + Randomf(-50.0f, 50.0f));
				element.transform.position.z = Randomf(0.0f, 50.0f);
				element.transform.scale = Vector3(Randomf(3.0f, 15.0f), Randomf(70.0f, 100.0f), 1.0f);
				element.color.a = 1;
				element.timer.Reset(display_interval);
			}
		}

	}
};

class SunLight : public Object, public ISeason
{
public:
	SunLight(int count)
	{
		name = "SunLight";
		overlay = true;

		particle = AddComponent<ParticleSystem>(count);
		particle->SetLayer(Layer::BULLET);
		particle->SetBehavior(&behavior);
		particle->pTexture = Texture::Get("sun_light");
		particle->pshader = PixelShader::Get("OverlayPS");
		particle->loop = true;
		particle->emission_rate = 1000.0f;
	}

	void Update(void) override
	{

		if (GetKeyboardTrigger(DIK_O))
		{
			overlay = !overlay;
			behavior.overlay = overlay;
			behavior.opacity_max = overlay ? 1.0f : 0.4f;
		}
	}

	void OnDraw(void) override
	{
		auto pDevice = Direct3D::GetDevice();

		if (overlay)
		{
			pDevice->SetPixelShader(particle->pshader->pD3DShader);
			pDevice->SetTexture(1, RenderTarget::Get("rt_main")->pDXTex);

			// αブレンドなし
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}

		else
		{
			// 加算合成
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		// ライト無効
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// Zバッファを無効
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	}

	void AfterDraw(void) override
	{
		auto pDevice = Direct3D::GetDevice();

		if (overlay)
		{
			pDevice->SetPixelShader(NULL);
			pDevice->SetTexture(1, NULL);
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		}
		else
		{
			// 通常ブレンド
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// ライト有効
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// Zバッファを有効
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	}

	void SetSummer(void) override
	{
		SetActive(true);
	}

	void SetWinter(void) override
	{
		SetActive(false);
	}
 
	ParticleSystem* particle;
	SunLightBehavior behavior;
	bool overlay;
};