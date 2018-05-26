#pragma once
#include <memory>
#include <unordered_map>
#include <string>

//=============================================================================
// リソースクラス定義
//=============================================================================

template<class T>
class Resource
{
public:
	static constexpr char* BasePath = "Data/";

	static T* Get(std::string name);
	static T* Load(std::string path);
	static void Release(std::string name);

protected:
	static void InsertToMap(T* ptr);
	static int ShowErrorMessage(const char* mesg);
	static std::unordered_map<std::string, std::unique_ptr<T>> name_map;


public:
	std::string name;
	Resource(std::string name) : name(name) {};
	void Release(void);
	
};

template<class T>
std::unordered_map<std::string, std::unique_ptr<T>> Resource<T>::name_map;


//=============================================================================
// 実装
//=============================================================================
template<class T>
inline T* Resource<T>::Load(std::string path)
{
	// 既存確認
	auto resource = Get(path);
	if (resource)
		return resource;

	// ロードする
	resource = T::_Load(path);
	if (resource)
	{
		name_map[resource->name].reset(resource);
		return resource;
	}

	// ロード失敗
	return nullptr;
}

template<class T>
inline T * Resource<T>::Get(std::string name)
{
	try
	{
		return name_map.at(name).get();
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}
}

template<class T>
inline void Resource<T>::Release(std::string name)
{
	name_map.erase(name);
}

template<class T>
inline void Resource<T>::InsertToMap(T * ptr)
{
	name_map[ptr->name] = this;
}

template<class T>
inline int Resource<T>::ShowErrorMessage(const char* mesg)
{
	TCHAR title[128];
	wsprintf(title, _T("%s エラー"), typeid(T).name()+5);
	return MessageBox(Window::GetHWnd(), mesg, title, MB_ABORTRETRYIGNORE | MB_ICONWARNING);
}


template<class T>
inline void Resource<T>::Release(void)
{
	Release(name);
}


