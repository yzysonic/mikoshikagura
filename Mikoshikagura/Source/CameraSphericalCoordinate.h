#pragma once
#include "Core/Core.h"

class CameraSphericalCoordinate : public Script
{
public:
	float phi;		// �����_�Ƃ̃Ӊ�]�p�i���݁j
	float theta;	// �����_�Ƃ̃Ɖ�]�p�i���݁j
	float distance;	// �����_�Ƃ̋���

	void Init(void) override;
	void Update(void) override;

private:
	Camera * camera;
};