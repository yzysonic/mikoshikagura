#pragma once
#include "Core/Core.h"
#include "CameraSphericalCoordinate.h"

class CameraPlay : public Script
{
public:
	void Init(void) override;
	void Update(void) override;

private:
	Camera* camera;
	CameraSphericalCoordinate* coordinate;
	Transform last_transform;
	float move_phi;
	float move_theta;
	float target_phi;
	float target_theta;
	float target_distance;
};
