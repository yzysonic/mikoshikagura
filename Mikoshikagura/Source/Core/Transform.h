#pragma once
#include "Vector.h"
#include <d3dx9math.h>

class Object;

class Transform
{
public:
	Object* object;
	Vector3 position;
	Vector3 scale;
	D3DXMATRIX mtx_world;

	Transform(void);
	Vector3 getRotation(void) const;
	void setRotation(Vector3 rotation);
	void setRotation(float x, float y, float z);
	Vector3 getUp(void);
	Vector3 getFront(void);
	void setUp(Vector3 up);
	void setFront(Vector3 front);
	void rotate(Vector3 angle);
	void rotate(float x, float y, float z);
	void lookAt(Vector3 const& target);
	void lookAt(Transform *target);
	void UpdateWorldMatrix(void);

private:
	Vector3 rotation;
	Vector3 up;
	Vector3 front;

	void updateVector(void);
};
