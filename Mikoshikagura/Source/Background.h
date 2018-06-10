#pragma once
#include "Core/Core.h"

class Background : public Object
{
public:
	Background(void);
	void Update(void) override;

private:
	RectPolygon2D * image;
	Transform* camera;
};