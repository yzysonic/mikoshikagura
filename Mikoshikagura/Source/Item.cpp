#include "Item.h"
#include "Holdable.h"

Item::Item(const char * texture_name, Vector2 size)
{
	name = "Item";
	type = ObjectType::Item;
	AddComponent<RectPolygon>(texture_name);
	collider = AddComponent<BoxCollider2D>();
	AddComponent<Holdable>();
	rigidbody = AddComponent<Rigidbody>();
	rigidbody->SetActive(false);
	SetSize((size == Vector2::zero) ? Texture::Get(texture_name)->size : size);
}

void Item::Update(void)
{
	last_position = transform.position;
}

void Item::OnCollisionStay(Object * other)
{
	if (other->type == ObjectType::Field && rigidbody->GetActive())
	{
		auto otherCollider = other->GetComponent<BoxCollider2D>();
		auto otherColliderPos = other->transform.position.toVector2() + otherCollider->offset;

		// íÖínîªíË
		if (last_position.y + collider->offset.y - 0.5f*collider->size.y >=
			otherColliderPos.y + 0.5f*otherCollider->size.y)
		{
			rigidbody->SetActive(false);
		}

		// è’ìÀâûìöÇÃèàóù
		auto diff = Vector2(0.0f, 0.0f);

		// ÇﬂÇËçûÇ›ÇÃó ÇåvéZÇ∑ÇÈ
		if (rigidbody->position.x < otherColliderPos.x)
		{
			diff.x = (rigidbody->position.x + collider->offset.x + 0.5f*collider->size.x) -
				(otherColliderPos.x - 0.5f*otherCollider->size.x);
			diff.x += 0.0001f;
		}
		else
		{
			diff.x = (rigidbody->position.x + collider->offset.x - 0.5f*collider->size.x) -
				(otherColliderPos.x + 0.5f*otherCollider->size.x);
			diff.x -= 0.0001f;
		}

		if (rigidbody->position.y < otherColliderPos.y)
		{
			diff.y = (rigidbody->position.y + collider->offset.y + 0.5f*collider->size.y) -
				(otherColliderPos.y - 0.5f*otherCollider->size.y);
		}
		else
		{
			diff.y = (rigidbody->position.y + collider->offset.y - 0.5f*collider->size.y) -
				(otherColliderPos.y + 0.5f*otherCollider->size.y);
		}

		// ÇﬂÇËçûÇÒÇæó ÇæÇØâüÇµï‘Ç∑
		if (fabsf(diff.x) <= fabsf(diff.y))
		{
			rigidbody->position.x -= diff.x;
			rigidbody->velocity.x = 0.0f;
			transform.position.x = rigidbody->position.x;
		}
		else
		{
			rigidbody->position.y -= diff.y;
			rigidbody->velocity.y = 0.0f;
			transform.position.y = rigidbody->position.y;
		}

	}

}

void Item::SetSize(Vector2 size)
{
	GetComponent<RectPolygon>()->SetSize(size);
	GetComponent<BoxCollider2D>()->size = size;
	this->size = size;
}

Vector2 Item::GetSize(void)
{
	return size;
}
