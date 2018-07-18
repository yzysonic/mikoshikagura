#pragma once
#include "Core/Core.h"
#include "CameraSphericalCoordinate.h"
#include "CameraSmoothFollow.h"
#include "CameraSnap.h"
#include "CameraLimit.h"

class MainCamera :public Camera
{
public:
	MainCamera(void);
	void SetTarget(Transform* target);
	void AddSnapper(Snapper* object);
	void RemoveSnapper(Snapper* object);

	//0627 çÌèúó\íË
	void AddSnapper(Transform* target) {};
	void RemoveSnapper(Transform* target) {};

private:
	CameraSphericalCoordinate* coordinate;
	CameraSmoothFollow* smooth;
	CameraSnap* snap;
	CameraLimit* limit;
};