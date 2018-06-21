#pragma once
#include "Core/Core.h"

class SunLight : public Object, public ISeason
{
public:
	SunLight(void)
	{
		name = "SunLight";
		overlay = true;

		transform.setRotation(0.0f, 0.0f, Deg2Rad(26.5f));

		polygon = AddComponent<RectPolygon>("sun_light", Layer::BULLET);
		polygon->vshader = VertexShader::Get("NormalVS");
		polygon->pshader = PixelShader::Get("OverlayPS");
		polygon->SetOpacity(0.0f);

		camera = RenderSpace::Get("default")->GetCamera(0);
		timer.Reset(Randomf(0.3f, 15.0f));
		display = false;
		opacity_max = overlay? 1.0f : 0.4f;
		opacity_min = 0.0f;
		display_interval = 2.5f;
	}

	void Update(void) override
	{
		timer++;

		if (display)
		{
			auto a = Lerpf(opacity_min, opacity_max, 0.5f*(1.0f - cosf(timer.Progress()*2.0f*PI)));

			polygon->SetOpacity(a);
			transform.position.y = camera->transform.position.y + 30.0f;

			if (timer.TimeUp())
			{
				display = false;
				timer.Reset(Randomf(0.3f, 15.0f));
			}

		}
		else
		{
			if (timer.TimeUp())
			{
				transform.position.x = camera->transform.position.x + (-30.0f+Randomf(-50.0f, 50.0f));
				transform.position.z = Randomf(0.0f, 50.0f);
				polygon->SetSize(Vector2(Randomf(3.0f, 15.0f), Randomf(70.0f, 100.0f)));
				display = true;
				timer.Reset(display_interval);
			}
		}

		if (GetKeyboardTrigger(DIK_O))
		{
			overlay = !overlay;
			opacity_max = overlay ? 1.0f : 0.4f;
		}
	}

	void OnDraw(void) override
	{
		auto pDevice = Direct3D::GetDevice();
		auto camera = RenderSpace::Get("default")->GetCamera(0);

		if (overlay)
		{
			transform.UpdateWorldMatrix();

			polygon->vshader->SetMatrix("WorldViewProj", transform.mtx_world*camera->getViewMatrix(false)*camera->getProjectionMatrix(false));
			pDevice->SetVertexShader(polygon->vshader->pD3DShader);
			pDevice->SetPixelShader(polygon->pshader->pD3DShader);

			pDevice->SetTexture(1, RenderTarget::Get("rt_main")->pDXTex);

			// ���u�����h�Ȃ�
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}

		else
		{
			// ���Z����
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// ���C�g�Ȃ�
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		}


		// Z�o�b�t�@�𖳌�
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	}

	void AfterDraw(void) override
	{
		auto pDevice = Direct3D::GetDevice();

		if (overlay)
		{
			pDevice->SetVertexShader(NULL);
			pDevice->SetPixelShader(NULL);
			pDevice->SetTexture(1, NULL);
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		}
		else
		{
			// �ʏ�u�����h
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// ���C�g����
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}

		// Z�o�b�t�@��L��
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
 
	FrameTimer timer;
	RectPolygon* polygon;
	Camera* camera;
	float opacity_max;
	float opacity_min;
	float display_interval;
	float init_x;
	bool display;
	bool overlay;
};