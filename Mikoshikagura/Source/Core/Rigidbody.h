#pragma once
#include "Component.h"
#include "Vector.h"

struct RigidbodyConstraints
{
	bool pos_x;
	bool pos_y;
	bool pos_z;
};

class Rigidbody : public Component
{
public:
	Vector3		position;
	Vector3		rotation;
	Vector3		velocity;
	Vector3		acceleration;	// �����x
	Vector3		force;			// ��
	Vector3		net_force;		// ����
	float		mass;			// ����
	float		bounciness;		// �e���W��
	bool useGravity;
	int listIndex;
	RigidbodyConstraints constraints;

	Rigidbody(void);
	~Rigidbody(void);
	void BindObject(Object* object) override;
	bool SetActive(bool value) override;

};