#include "Transform.h"
#include "Object.h"
#include "Math.h"

Transform::Transform(void)
{

	this->object = nullptr;
	this->position = Vector3(0, 0, 0);
	this->rotation = Vector3(0, 0, 0);
	this->scale = Vector3(1, 1, 1);
	this->up = Vector3(0, 1, 0);
	this->front = Vector3(0, 0, -1);

	D3DXMatrixIdentity(&this->mtx_world);

}

Vector3 Transform::getRotation(void) const
{
	return this->rotation;
}

void Transform::setRotation(Vector3 rotation)
{
	this->rotation = rotation;
	this->updateVector();
}

void Transform::setRotation(float x, float y, float z)
{
	this->rotation.x = x;
	this->rotation.y = y;
	this->rotation.z = z;
	this->updateVector();

}

Vector3 Transform::getUp(void)
{
	return this->up;
}

Vector3 Transform::getFront(void)
{
	return this->front;
}

void Transform::setUp(Vector3 up)
{
	this->up = up.normalized();
	this->rotation.z = atan2f(up.y, up.x) - PI / 2;
}

void Transform::setFront(Vector3 front)
{
	this->front = front.normalized();
	this->rotation.y = atan2f(-front.z, front.x) - PI / 2;

}

void Transform::rotate(Vector3 angle)
{
	this->rotation += angle;
	this->updateVector();
}

void Transform::rotate(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
	if (rotation.x > 2 * PI)
		rotation.x -= (int)(rotation.x / 2 / PI) * 2 * PI;
	if (rotation.y > 2 * PI)
		rotation.y -= (int)(rotation.y / 2 / PI) * 2 * PI;
	if (rotation.z > 2 * PI)
		rotation.z -= (int)(rotation.z / 2 / PI) * 2 * PI;
	this->updateVector();
}

void Transform::lookAt(Vector3 const & target)
{
	Vector3 dis = target - this->position;

	this->rotation.y = atan2f(-dis.z, dis.x) - PI / 2.0f;
	this->updateVector();
}

void Transform::lookAt(Transform * target)
{
	lookAt(target->position);
}

void Transform::UpdateWorldMatrix(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ƒXƒP[ƒ‹‚ðŒvŽZ
	D3DXMatrixScaling(&mtxScl, this->object->transform.scale.x, this->object->transform.scale.y, this->object->transform.scale.z);
	// ‰ñ“]‚ðŒvŽZ
	D3DXMatrixRotationYawPitchRoll(&mtxRot, this->object->transform.getRotation().y, this->object->transform.getRotation().x, this->object->transform.getRotation().z);
	// •½sˆÚ“®‚ðŒvŽZ
	D3DXMatrixTranslation(&mtxTranslate, this->object->transform.position.x, this->object->transform.position.y, this->object->transform.position.z);

	this->mtx_world = mtxScl*mtxRot*mtxTranslate;
}

void Transform::updateVector(void)
{
	this->up.x = cosf(rotation.z + PI / 2);
	this->up.y = sinf(rotation.z + PI / 2);
	this->front.x = cosf(rotation.y + 0.5f*PI);
	this->front.z = -sinf(rotation.y + 0.5f*PI);
}