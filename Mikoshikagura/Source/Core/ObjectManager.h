#pragma once

#include "Object.h"
#include "Singleton.h"
#include <vector>
#include <memory>

class ObjectManager : public Singleton<ObjectManager>
{
	friend class Object;
public:
	static void Create(void);
	static void Destroy(void);
	static void Update(void);
	static void KillObject(void);
	static Object* GetObjectByType(ObjectType type);

private:
	std::vector<std::unique_ptr<Object>> objectList;
	std::vector<Object*> killList;

	void* NewObject(std::size_t size);
	void* NewObjectArray(std::size_t size);
#ifdef _DEBUG
	void* NewObject(std::size_t size, int _BlockUse, char const* _FileName, int _LineNumber);
	void* NewObjectArray(std::size_t size, int _BlockUse, char const* _FileName, int _LineNumber);
#endif
	void InitObject(Object* object);
	void AddKill(Object* obj);
	void DeleteObject(Object* obj);

};