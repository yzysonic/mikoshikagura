#pragma once
#include "Core/Core.h"

class CameraPlay : public Script
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	Camera* camera;
	Transform last_transform;
	float phi;				// �����_�Ƃ̃Ӊ�]�p�i���݁j
	float theta;			// �����_�Ƃ̃Ɖ�]�p�i���݁j
	float move_phi;			// �Ӊ�]�p�̈ړ���
	float move_theta;		// �Ɖ�]�p�̈ړ���
	float target_phi;
	float target_theta;
	float target_dis;		// �ڕW����
	float dis;				// �����_�Ƃ̋���

	void MoveCamera(void);
};
