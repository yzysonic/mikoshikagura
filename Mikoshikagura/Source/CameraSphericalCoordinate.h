#pragma once
#include "Core/Core.h"

class CameraSphericalCoordinate : public Script
{
public:
	float phi;		// 注視点とのφ回転角（現在）
	float theta;	// 注視点とのθ回転角（現在）
	float distance;	// 注視点との距離

	void Init(void) override;
	void Update(void) override;

private:
	Camera * camera;
};