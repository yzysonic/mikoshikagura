#pragma once
#include "Core/Core.h"
#include "CameraSphericalCoordinate.h"
#include "CameraSmoothFallow.h"
#include "CameraPlay.h"

class FreeCamera : public Camera
{
public:
	FreeCamera(Camera* camera = nullptr);

private:
	CameraSphericalCoordinate * coordinate;
	CameraSmoothFallow* smooth;
	CameraPlay* play;
};