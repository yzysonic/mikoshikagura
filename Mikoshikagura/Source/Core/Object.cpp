#include "Object.h"
#include "ObjectManager.h"
#include "Scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

Object::Object()
{
	this->transform.object = this;
	this->type			= ObjectType::Object;
	this->isActive		= true;
	this->name			= "Object";
	this->group			= 0;
	this->kill_flag		= false;
	this->scene			= nullptr;
	this->sceneIndex	= -1;
	this->objectIndex	= -1;
	this->dontStopWhenPause = false;

	ObjectManager::GetInstance()->InitObject(this);

	this->SetActive(true);
}

Object::Object(Vector3 position, Vector3 rotation) : Object()
{
	this->transform.position = position;
	this->transform.setRotation(rotation);
}

Object::~Object()
{
	if (this->component_map.size() > 0)
	{
		for (auto &components : this->component_map)
		{
			for (auto &component : components.second)
			{
				component.reset();
			}
		}
	}

	if (this->scene)
		this->scene->RemoveObject(this);
}

std::vector<Component*> Object::GetComponents(void)
{
	std::vector<Component*> list;
	for (auto& components : component_map)
	{
		for (auto& component : components.second)
		{
			list.emplace_back(component.get());
		}
	}
	return list;
}

std::vector<Script*> Object::GetScripts(void)
{
	return scripts;
}

void Object::SetActive(bool value)
{
	if (this->isActive == value)
		return;

	this->isActive = value;

	if (value)
	{
		int i = 0;
		for (auto &components : this->component_map)
		{
			for (auto &component : components.second)
			{
				component->SetActive(this->component_actives[i++]);
			}
		}
	}
	else
	{
		this->component_actives.clear();

		for (auto &components : this->component_map)
		{
			for (auto &component : components.second)
			{
				this->component_actives.push_back(component->active);
				component->SetActive(false);
			}
		}
	}
	
}

bool Object::GetActive(void)
{
	return this->isActive;
}

void Object::Destroy(void)
{
	if (this->kill_flag == false)
	{
		this->SetActive(false);
		this->Uninit();
		ObjectManager::GetInstance()->AddKill(this);
	}
}


void * Object::operator new(std::size_t size)
{
	return ObjectManager::GetInstance()->NewObject(size);
}

void * Object::operator new[](std::size_t size)
{
	return nullptr;
}

void Object::operator delete(void * ptr) noexcept
{

	if (ptr == nullptr)
		return;
	
	ObjectManager::GetInstance()->DeleteObject((Object*)ptr);
}

void Object::operator delete[](void * ptr) noexcept
{
}

#ifdef _DEBUG

void * Object::operator new(std::size_t size, int _BlockUse, char const* _FileName, int _LineNumber)
{
	return ObjectManager::GetInstance()->NewObject(size, _BlockUse, _FileName, _LineNumber);
}

void * Object::operator new[](std::size_t size, int _BlockUse, char const * _FileName, int _LineNumber)
{
	return ObjectManager::GetInstance()->NewObjectArray(size, _BlockUse, _FileName, _LineNumber);
}

void Object::operator delete(void * ptr, int _BlockUse, char const * _FileName, int _LineNumber) noexcept
{
	delete (Object*)ptr;
}

void Object::operator delete[](void * ptr, int _BlockUse, char const * _FileName, int _LineNumber) noexcept
{
	delete (Object*)ptr;
}

#endif