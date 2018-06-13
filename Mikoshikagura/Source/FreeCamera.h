#pragma once
#include "Core/Core.h"
#include "CameraSphericalCoordinate.h"
#include "CameraSmoothFollow.h"
#include "CameraPlay.h"

class FreeCamera : public Camera
{
public:
	FreeCamera(Camera* camera = nullptr);

private:
	CameraSphericalCoordinate * coordinate;
	CameraSmoothFollow* smooth;
	CameraPlay* play;
};