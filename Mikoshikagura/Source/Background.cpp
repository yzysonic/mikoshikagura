#include "Background.h"

Background::Background(void)
{
	transform.position.y += 100.0f;
	image_layer0 = AddComponent<RectPolygon2D>("background_summer_layer0", Layer::BG_00);
	image_layer0->SetSize(1300.0f, 975.0f);
	image_layer1 = AddComponent<RectPolygon2D>("background_summer_layer1", Layer::BG_01);
	image_layer1->SetSize(1600.0f, 1200.0f);
	image_layer2 = AddComponent<RectPolygon2D>("background_summer_layer2", Layer::BG_02);
	image_layer2->SetSize(1900.0f, 1425.0f);
	camera = &RenderSpace::Get("default")->GetCamera(0)->transform;
}

void Background::Update(void)
{
	auto offset = (camera->position*0.001f).toVector2();
	offset.y *= -1.0f;

	offset.y -= 0.05f;
	image_layer0->vertex[0].uv = offset;
	image_layer0->vertex[1].uv = Vector2(1.0f, 0.0f) + offset;
	image_layer0->vertex[2].uv = Vector2(0.0f, 1.0f) + offset;
	image_layer0->vertex[3].uv = Vector2(1.0f, 1.0f) + offset;

	offset.y += 0.075f;
	image_layer1->vertex[0].uv = offset * 1.5f;
	image_layer1->vertex[1].uv = Vector2(1.0f, 0.0f) + offset * 1.5f;
	image_layer1->vertex[2].uv = Vector2(0.0f, 1.0f) + offset * 1.5f;
	image_layer1->vertex[3].uv = Vector2(1.0f, 1.0f) + offset * 1.5f;

	offset.y += 0.015f;
	image_layer2->vertex[0].uv = offset * 2.0f;
	image_layer2->vertex[1].uv = Vector2(1.0f, 0.0f) + offset * 2.0f;
	image_layer2->vertex[2].uv = Vector2(0.0f, 1.0f) + offset * 2.0f;
	image_layer2->vertex[3].uv = Vector2(1.0f, 1.0f) + offset * 2.0f;
}

void Background::SetSummer(void)
{
	image_layer0->SetTexture("background_summer_layer0");
	image_layer0->SetSize(1300.0f, 975.0f);

	image_layer1->SetTexture("background_summer_layer1");
	image_layer1->SetSize(1600.0f, 1200.0f);

	image_layer2->SetTexture("background_summer_layer2");
	image_layer2->SetSize(1900.0f, 1425.0f);
}

void Background::SetWinter(void)
{
	image_layer0->SetTexture("background_winter_layer0");
	image_layer0->SetSize(1300.0f, 975.0f);

	image_layer1->SetTexture("background_winter_layer1");
	image_layer1->SetSize(1600.0f, 1200.0f);

	image_layer2->SetTexture("background_winter_layer2");
	image_layer2->SetSize(1900.0f, 1425.0f);
}
