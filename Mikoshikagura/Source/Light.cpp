#include "Light.h"
#include "Core/Direct3D.h"

D3DLIGHT9 Light::light[NUM_LIGHT];
smart_ptr<Light> Light::instance;

//=============================================================================
// ライトの初期化処理
//=============================================================================
void Light::Init(void)
{
	if (instance == nullptr)
		instance.reset(new Light);

	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&light[0], sizeof(D3DLIGHT9));

	// ライト0のタイプの設定
	light[0].Type = D3DLIGHT_DIRECTIONAL;

	// ライト0の拡散光の設定
	light[0].Diffuse = D3DXCOLOR(0xffffffff);
	light[0].Ambient = D3DXCOLOR(0xffffffff);
	
	// ライト0の方向の設定
	light[0].Direction = Vector3(968.0f, -934.0f, 2689.0f).normalized();

	// ライト0をレンダリングパイプラインに設定
	pDevice->SetLight(0, &light[0]);

	// ライト0を使用使用状態に
	pDevice->LightEnable(0, TRUE);


	// ライティングモードをON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(0xffffffff));
	//pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(0xfff6cb6e));
}

Light* Light::GetInstance(void)
{
	return instance.get();
}

HRESULT Light::OnLostDevice(void)
{
	return S_OK;
}

HRESULT Light::OnResetDevice(void)
{
	Init();
	return S_OK;
}
