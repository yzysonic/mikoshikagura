#include "MainCamera.h"

MainCamera::MainCamera(void)
{
	this->far_z = 1000.0f;
	this->transform.position = Vector3(0.0f, 0.0f, -150.0f);
	this->fov = Deg2Rad(60.0f);
}
