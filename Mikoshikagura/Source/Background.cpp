#include "Background.h"

Background::Background(void)
{
	transform.position.y -= 40.0f;
	image = AddComponent<RectPolygon2D>("background", Layer::BG_00);
	image->SetSize(1300.0f, 975.0f);
	camera = &RenderSpace::Get("default")->GetCamera(0)->transform;
}

void Background::Update(void)
{
	auto offset = (camera->position*0.001f).toVector2();
	offset.y *= -1.0f;
	image->vertex[0].uv = offset;
	image->vertex[1].uv = Vector2(1.0f, 0.0f) + offset;
	image->vertex[2].uv = Vector2(0.0f, 1.0f) + offset;
	image->vertex[3].uv = Vector2(1.0f, 1.0f) + offset;
}
