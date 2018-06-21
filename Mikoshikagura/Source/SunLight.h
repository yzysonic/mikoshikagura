#pragma once
#include "Core/Core.h"
#include "ISeason.h"

class SunLight : public Object, public ISeason
{
public:
	float opacity_max;
	float opacity_min;
	float display_interval;

	SunLight(void);
	void Update(void) override;
	void OnDraw(void) override;
	void AfterDraw(void) override;

	void SetSummer(void) override
	{
		SetActive(true);
	}
	void SetWinter(void) override
	{
		SetActive(false);
	}

private:
	FrameTimer timer;
	RectPolygon* polygon;
	Camera* camera;
	bool display;

};