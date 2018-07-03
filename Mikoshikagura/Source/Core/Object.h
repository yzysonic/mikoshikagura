#pragma once

#include "Common.h"
#include "ObjectType.h"
#include "Transform.h"
#include "Component.h"
#include "Script.h"
#include "Vector.h"

#include <string>
#include <memory>

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Transform;
class Scene;

class Object
{
	friend class ObjectManager;
	friend class Scene;

public:
	template<class T>
	static void Destroy(T*& object);
	template<class T>
	static void Destroy(std::unique_ptr<T> &object);

public:
	Transform transform;
	ObjectType type;
	Scene* scene;
	int objectIndex;
	int sceneIndex;
	std::string name;
	int group;
	bool dontStopWhenPause;

	Object(void);
	Object(Vector3 position, Vector3 rotation);
	virtual ~Object(void);

	virtual void Update(void) {};
	virtual void Uninit(void) {};
	virtual void OnDraw(void) {};
	virtual void OnCollisionEnter(Object* otehr) {};
	virtual void OnCollisionStay(Object* other) {};
	virtual void OnCollisionExit(Object* other) {};
	virtual void AfterDraw(void) {};

	template<class T, class... Args>
	T* AddComponent(Args&&... args);
	template<class T>
	T* GetComponent(void);
	std::vector<Component*> GetComponents(void);
	std::vector<Script*> GetScripts(void);

	virtual void SetActive(bool value);
	bool GetActive(void);
	void Destroy(void);

	static void* operator new(std::size_t);
	static void* operator new[](std::size_t);
	static void operator delete(void*) noexcept;
	static void operator delete[](void*) noexcept;
#ifdef _DEBUG
	static void* operator new(std::size_t, int _BlockUse, char const* _FileName, int _LineNumber);
	static void* operator new[](std::size_t, int _BlockUse, char const* _FileName, int _LineNumber);
	static void operator delete(void*, int _BlockUse, char const* _FileName, int _LineNumber) noexcept;
	static void operator delete[](void*, int _BlockUse, char const* _FileName, int _LineNumber) noexcept;

#endif

protected:
	bool isActive;
#ifndef _DEBUG
	std::unordered_map<size_t, std::vector<std::unique_ptr<Component>>> component_map;
#else
	std::unordered_map<std::string, std::vector<std::unique_ptr<Component>>> component_map;
#endif
	std::vector<Script*> scripts;

private:
	bool kill_flag;
	std::vector<bool> component_actives;
};

template<class T>
inline void Object::Destroy(T *& object)
{
	static_assert(std::is_base_of<Object, T>::value, "Not a subclass of Object.");

	if (object == nullptr) return;
	object->Destroy();
	object = nullptr;
}

template<class T>
inline void Object::Destroy(std::unique_ptr<T>& object)
{
	static_assert(std::is_base_of<Object, T>::value, "Not a subclass of Object.");

	object->Destrory();
	object.release();
}

template<class T, class... Args>
inline T * Object::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of<Component, T>::value, "Not a subclass of Component.");

	Component* component = new T(args...);
	component->BindObject(this);
	if(component->active_on_load && this->isActive)
		component->SetActive(true);

#ifndef _DEBUG
	this->component_map[typeid(T).hash_code()].emplace_back(component);
#else
	component->name = TypeName<T>();
	this->component_map[component->name].emplace_back(component);
#endif

	if (component->type == ComponentType::Script)
		this->scripts.push_back(dynamic_cast<Script*>(component));

	return dynamic_cast<T*>(component);
}

template<class T>
inline T * Object::GetComponent(void)
{
	static_assert(std::is_base_of<Component, T>::value, "Not a subclass of Component.");

#ifndef _DEBUG
	auto it = this->component_map.find(typeid(T).hash_code());
#else
	auto it = this->component_map.find(TypeName<T>());
#endif

	if (it == this->component_map.end())
		return nullptr;

	return dynamic_cast<T*>(it->second.front().get());
}
