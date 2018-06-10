#pragma once
#include "Core/Core.h"
#include "CameraSphericalCoordinate.h"
#include "CameraSmoothFallow.h"
#include "CameraSnap.h"

class MainCamera :public Camera
{
public:
	MainCamera(void);
	void SetTarget(Transform* target);
	void AddSnapper(Transform* target);
	void RemoveSnapper(Transform* target);

private:
	CameraSphericalCoordinate* coordinate;
	CameraSmoothFallow* smooth;
	CameraSnap* snap;
};