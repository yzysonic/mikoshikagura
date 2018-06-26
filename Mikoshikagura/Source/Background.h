#pragma once
#include "Core/Core.h"
#include "ISeason.h"

class Background : public Object, public ISeason
{
public:
	Background(void);
	void Update(void) override;
	void SetSummer(void) override;
	void SetWinter(void) override;

private:
	RectPolygon2D * image;
	Transform* camera;
};