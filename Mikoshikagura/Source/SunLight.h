#pragma once
#include "Core/Core.h"

class SunLight : public Object
{
public:
	SunLight(void)
	{
		polygon = AddComponent<RectPolygon>("sun_light2", Layer::BULLET);
		polygon->SetOpacity(0.0f);

		transform.position.y = 150.0f;
		transform.setRotation(0.0f, 0.0f, Deg2Rad(26.5f));
		timer.Reset(Randomf(0.3f, 1.0f));
		display = false;
		opacity_max = 0.4f;
		opacity_min = 0.0f;
		display_interval = 4.0f;
	}

	void Update(void) override
	{
		timer++;

		if (display)
		{
			auto a = Lerpf(opacity_min, opacity_max, 0.5f*(1.0f - cosf(timer.Progress()*2.0f*PI)));

			polygon->SetOpacity(a);

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
				transform.position.x = RenderSpace::Get("default")->GetCamera(0)->transform.position.x + (Randomf(-50.0f, 50.0f));
				transform.position.z = Randomf(0.0f, 20.0f);
				polygon->SetSize(Vector2(Randomf(3.0f, 20.0f), 200.0f));
				display = true;
				timer.Reset(display_interval);
			}
		}

	}

	void OnDraw(void) override
	{
		auto pDevice = Direct3D::GetDevice();

		// Zバッファを無効
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		// 加算合成
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// ライトなし
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	}

	void AfterDraw(void) override
	{
		auto pDevice = Direct3D::GetDevice();

		// Zバッファを有効
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

		// 通常ブレンド
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// ライトあり
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	}

	FrameTimer timer;
	RectPolygon* polygon;
	float opacity_max;
	float opacity_min;
	float display_interval;
	float init_x;
	bool display;
};