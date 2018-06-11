#include "Physics.h"
#include "Common.h"
#include "Object.h"
#include "Time.h"
#include "Collision.h"

using namespace std;

void Physics::Create(void)
{
	Singleton::Create();

	m_pInstance->gravaty = Vector3(0, -9.8f, 0);
	m_pInstance->colliderList.reserve(SystemParameters::ObjectMax);
	m_pInstance->rigidbodyList.reserve(SystemParameters::ObjectMax);

	CollisionInit();
}

void Physics::Destroy(void)
{
	m_pInstance->colliderList.clear();
	m_pInstance->rigidbodyList.clear();

	Singleton::Destroy();
}

void Physics::Update()
{
	m_pInstance->updateDynamics();
	m_pInstance->testCollisions();
}

void Physics::addCollider(Collider* collider)
{
	this->colliderList.push_back(collider);
	collider->listIndex = this->colliderList.size() - 1;
}

void Physics::removeCollider(Collider * collider)
{
	size_t index = collider->listIndex;

	// collisionSet����폜
	for (auto pair = this->collisionSet.begin(); pair != this->collisionSet.end();)
	{
		if (pair->first == collider || pair->second == collider)
		{
			pair->first->object->OnCollisionExit(pair->second->object);
			pair->second->object->OnCollisionExit(pair->first->object);

			auto erase_pair = pair;
			pair++;
			this->collisionSet.erase(erase_pair);
		}
		else
		{
			pair++;
		}
	}

	// colliderList����폜
	if (index < this->colliderList.size() - 1)
	{
		this->colliderList[index] = this->colliderList.back();
		this->colliderList[index]->listIndex = index;
	}
	this->colliderList.pop_back();

	collider->listIndex = -1;

}

void Physics::addRigidbody(Rigidbody* rigidbody)
{
	this->rigidbodyList.push_back(rigidbody);
	rigidbody->listIndex = this->rigidbodyList.size() - 1;
}

void Physics::removeRigidbody(Rigidbody * rigidbody)
{

	size_t index = rigidbody->listIndex;

	if (index < this->rigidbodyList.size() - 1)
	{
		this->rigidbodyList[index] = this->rigidbodyList.back();
		this->rigidbodyList[index]->listIndex = index;
	}
	this->rigidbodyList.pop_back();

	rigidbody->listIndex = -1;
}

void Physics::setGravity(Vector3 value)
{
	this->gravaty = value;
}

Vector3 Physics::getGravity(void)
{
	return this->gravaty;
}


void Physics::updateDynamics()
{
	float dt = Time::DeltaTime();

	for(auto rb : this->rigidbodyList)
	{

		Vector3 a = rb->force / rb->mass;

		// ���͂̌v�Z
		rb->net_force = rb->force + rb->useGravity*this->gravaty*rb->mass;

		// �����v�Z
		rb->position		+= rb->velocity*dt + 0.5f*rb->acceleration*dt*dt;	//�ʒu�̍X�V
		rb->velocity		+= 0.5f*rb->acceleration*dt;						//���x�̍X�V(1)
		rb->acceleration	= rb->net_force / rb->mass;							//�͂̌v�Z
		rb->velocity		+= 0.5f*rb->acceleration*dt;						//���x�̍X�V(2)

		// ������I�u�W�F�N�g�֔��f
		rb->object->transform.position = rb->position;
		rb->object->transform.setRotation(rb->rotation);

	}

}

void Physics::testCollisions()
{
	Collision collision;

	for (size_t i = 0; i < this->colliderList.size(); i++)
	{
		auto a = this->colliderList[i];

		for (size_t j = i + 1; j < this->colliderList.size(); j++)
		{
			auto b = this->colliderList[j];

			if (a->object->type == b->object->type)
				continue;

			auto pair = std::pair<Collider*, Collider*>(a, b);

			// �Փ˂����ꍇ
			if (CollisionTest(a, b, &collision))
			{
				// �Փ˂̊J�n���m
				if (collisionSet.find(pair) == collisionSet.end())
				{
					collisionSet.insert(pair);
					a->object->OnCollisionEnter(b->object);
					b->object->OnCollisionEnter(a->object);
				}

				a->object->OnCollisionStay(b->object);
				b->object->OnCollisionStay(a->object);
			}

			// �Փ˂��Ȃ������ꍇ
			else
			{
				// �Փ˂̏I�����m
				if (collisionSet.find(pair) != collisionSet.end())
				{
					collisionSet.erase(pair);
					a->object->OnCollisionExit(b->object);
					b->object->OnCollisionExit(a->object);
				}
			}
			
		}
	}
}
