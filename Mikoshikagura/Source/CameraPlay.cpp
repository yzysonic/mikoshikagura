#include "CameraPlay.h"

void CameraPlay::Init(void)
{
	camera		= dynamic_cast<Camera*>(object);
	coordinate	= camera->GetComponent<CameraSphericalCoordinate>();

	target_theta	= coordinate->theta;
	target_phi		= coordinate->phi;
	target_distance = coordinate->distance;

	move_theta	= 0.0f;
	move_phi	= 0.0f;
}

void CameraPlay::Update(void)
{
	Vector3 move = Vector3(0.0f, 0.0f, 0.0f);

	// マウス操作
#ifdef IMGUI
	if(!ImGui::GetIO().WantCaptureMouse){
#endif

	if (IsMouseLeftPressed())
	{
		// 移動
		if (GetKeyboardPress(DIK_LSHIFT))
		{
			move.x = -GetMouseMoveX() / 5.0f;
			move.y = GetMouseMoveY() / 5.0f;
		}
		// 回転
		else
		{
			move_phi	= -GetMouseMoveX() / 500.0f;
			move_theta	= -GetMouseMoveY() / 500.0f;
		}
	}

	// ズーム
	target_distance += -GetMouseMoveZ() / 12.0f;

#ifdef IMGUI
	}
#endif


	// キーボード操作
	// 移動
	if (GetKeyboardPress(DIK_LSHIFT))
	{
		if (GetKeyboardPress(DIK_LEFT))
			move.x = -2.0f;
		if (GetKeyboardPress(DIK_RIGHT))
			move.x = 2.0f;
		if (GetKeyboardPress(DIK_UP))
			move.y = 2.0f;
		if (GetKeyboardPress(DIK_DOWN))
			move.y = -2.0f;
	}
	// 回転
	else
	{
		if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT))
			move_phi = 0.005f;
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT))
			move_phi = -0.005f;
		if (GetKeyboardPress(DIK_UP) || IsButtonPressed(0, BUTTON_UP))
			move_theta = 0.005f;
		if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, BUTTON_DOWN))
			move_theta = -0.005f;
	}

	// ゲームパッド操作
	// 回転
	Vector2 pad_input_r(GetPadRX(), GetPadRY());
	if (pad_input_r.sqrLength() >= 0.05f && !IsButtonPressed(0, BUTTON_L1))
	{
		move_phi	= -pad_input_r.x*0.03f;
		move_theta	= -pad_input_r.y*0.03f;
	}

	// ズーム
	if (IsButtonPressed(0, BUTTON_L1))
		target_distance += GetPadRY()*10.0f;

	// 距離の計算
	target_distance = fmaxf(1.0f, target_distance);
	coordinate->distance = coordinate->distance + (target_distance - coordinate->distance)*0.15f;

	// 回転の計算
	if (Vector2(move_phi, move_theta).sqrLength() != 0.0f)
	{
		target_phi		+= move_phi;
		target_theta	+= move_theta;

		move_phi	= 0.0f;
		move_theta	= 0.0f;

		//target_theta = clamp(target_theta, 5.0f, 95.0f);
	}

	coordinate->phi		= Lerpf(coordinate->phi, target_phi, 0.065f);
	coordinate->theta	= Lerpf(coordinate->theta, target_theta, 0.065f);

	// 移動の計算
	if (move.x != 0.0f || move.y != 0.0f)
	{
		auto x = -move.x*cosf(-coordinate->phi + 0.5f*PI) - move.z*sinf(-coordinate->phi + 0.5f*PI);
		move.z =  move.x*sinf(-coordinate->phi + 0.5f*PI) - move.z*cosf(-coordinate->phi + 0.5f*PI);
		move.x = x;

		camera->at += move;
	}

}