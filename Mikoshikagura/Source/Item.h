#pragma once
#include "Core\Core.h"

class Item : public Object
{
public:
	Item(const char* texture_name, Vector2 size = Vector2::zero);
	void Update(void) override;
	void OnCollisionStay(Object* other) override;
	void SetSize(Vector2 size);
	Vector2 GetSize(void);


private:
	Vector2 size;
	Vector3 last_position;
	Rigidbody* rigidbody;
	BoxCollider2D* collider;
};