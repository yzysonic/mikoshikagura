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
	opacity_max = 1.0f;
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
	auto camera = RenderSpace::Get("default")->GetCamera(0);

	transform.UpdateWorldMatrix();

	polygon->vshader->SetMatrix("WorldViewProj", transform.mtx_world*camera->getViewMatrix(false)*camera->getProjectionMatrix(false));
	pDevice->SetVertexShader(polygon->vshader->pD3DShader);
	pDevice->SetPixelShader(polygon->pshader->pD3DShader);

	pDevice->SetTexture(1, RenderTarget::Get("rt_main")->pDXTex);

	// ���u�����h�Ȃ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// Z�o�b�t�@�𖳌�
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

}

void SunLight::AfterDraw(void)
{
	auto pDevice = Direct3D::GetDevice();

	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);
	pDevice->SetTexture(1, NULL);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// Z�o�b�t�@��L��
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}