#include "SunLight.h"

SunLight::SunLight(void)
{
	name = "SunLight";

	transform.setRotation(0.0f, 0.0f, Deg2Rad(26.5f));

	polygon = AddComponent<RectPolygon>("sun_light", Layer::BULLET);
	polygon->vshader = VertexShader::Get("NormalVS");
	polygon->pshader = PixelShader::Get("OverlayPS");
	polygon->SetOpacity(0.0f);

	camera = RenderSpace::Get("default")->GetCamera(0);
	timer.Reset(Randomf(0.3f, 15.0f));
	display = false;
	opacity_max = 0.4f;
	opacity_min = 0.0f;
	display_interval = 2.5f;
}

void SunLight::Update(void)
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
			transform.position.x = camera->transform.position.x + (-30.0f + Randomf(-50.0f, 50.0f));
			transform.position.z = Randomf(0.0f, 50.0f);
			polygon->SetSize(Vector2(Randomf(3.0f, 15.0f), Randomf(70.0f, 100.0f)));
			display = true;
			timer.Reset(display_interval);
		}
	}

}

void SunLight::OnDraw(void)
{
	auto pDevice = Direct3D::GetDevice();

	// αブレンドなし
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// Zバッファを無効
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// 加算合成
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ライティングを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void SunLight::AfterDraw(void)
{
	auto pDevice = Direct3D::GetDevice();

	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
