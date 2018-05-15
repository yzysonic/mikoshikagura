#include "CameraPlay.h"

void CameraPlay::Init(void)
{
	camera = dynamic_cast<Camera*>(object);
	Vector3 offset = camera->transform.position - camera->at;

	target_theta =
	theta = atan2f(sqrtf(offset.x*offset.x + offset.z*offset.z), offset.y);

	target_phi =
	phi = atan2f(offset.z, offset.x);

	move_theta = 0.0f;
	move_phi = 0.0f;
	dis = offset.length();
	target_dis = dis;
}

void CameraPlay::Update(void)
{
	MoveCamera();

	// ���W�ϊ�
	camera->transform.position.y = dis * cosf(theta);
	camera->transform.position.x = dis * sinf(theta) * cosf(phi);
	camera->transform.position.z = dis * sinf(theta) * sinf(phi);
	camera->transform.position += camera->at;

}

void CameraPlay::Uninit(void)
{
}


void CameraPlay::MoveCamera(void)
{
	Vector3 move = Vector3(0.0f, 0.0f, 0.0f);

	// �}�E�X����
	if (IsMouseLeftPressed())
	{
		// �ړ�
		if (GetKeyboardPress(DIK_LSHIFT))
		{
			move.x = -GetMouseMoveX() / 5.0f;
			move.y = GetMouseMoveY() / 5.0f;
		}
		// ��]
		else
		{
			move_phi = -GetMouseMoveX() / 500.0f;
			move_theta = -GetMouseMoveY() / 500.0f;
		}
	}

	// �Y�[��
	target_dis += -GetMouseMoveZ() / 12.0f;


	// �L�[�{�[�h����
	// �ړ�
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
	// ��]
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

	// �Q�[���p�b�h����
	// ��]
	Vector2 pad_input_r(GetPadRX(), GetPadRY());
	if (pad_input_r.sqrLength() >= 0.05f && !IsButtonPressed(0, BUTTON_L1))
	{
		move_phi = -pad_input_r.x*0.03f;
		move_theta = -pad_input_r.y*0.03f;
	}

	// �Y�[��
	if(IsButtonPressed(0, BUTTON_L1))
		target_dis += GetPadRY()*10.0f;

#ifndef _DEBUG
	if (target_dis < 30.0f)
		target_dis = 30.f;
	if (target_dis > 100.0f)
		target_dis = 100.0f;
#endif

	dis = dis + (target_dis - dis)*0.15f;

	if(Vector2(move_phi, move_theta).sqrLength() != 0.0f)
	{
		target_phi += move_phi;
		target_theta += move_theta;

		move_phi = 0.0f;
		move_theta = 0.0f;

		if (target_theta < Deg2Rad(30.0f))
			target_theta = Deg2Rad(30.0f);
		if (target_theta > Deg2Rad(85.0f))
			target_theta = Deg2Rad(85.0f);

	}

	phi = Lerpf(phi, target_phi, 0.065f);
	theta = Lerpf(theta, target_theta, 0.065f);

	if (move.x != 0.0f || move.y != 0.0f)
	{

		float x = -move.x*cosf(-phi + 0.5f*PI) - move.z*sinf(-phi + 0.5f*PI);
		move.z = move.x*sinf(-phi + 0.5f*PI) - move.z*cosf(-phi + 0.5f*PI);
		move.x = x;

		camera->at += move;
	}

}
