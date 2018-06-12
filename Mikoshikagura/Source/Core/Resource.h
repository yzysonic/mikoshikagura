#pragma once
#include "Common.h"
#include "Window.h"
#include <unordered_map>

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
	static int ShowErrorMessage(const char* mesg, const char* extra);
	static std::string GetFullPath(std::string short_path);
	static std::unordered_map<std::string, std::unique_ptr<T>> name_map;


public:
	std::string name;
	std::string file_extension;
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
	// 拡張子処理
	auto name	= path;
	auto ext	= std::string(T::DefaultExtension);
	auto dotPos = path.find(".");

	if (dotPos != std::string::npos)
	{
		name = path.substr(0, dotPos);
		ext = path.substr(dotPos, path.size());
	}

	// 既存確認
	auto resource = Get(name);

	if (resource)
		return resource;

	// ロードする
	resource = T::InternalLoad(name, ext);

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
	name_map[ptr->name].reset(ptr);
}

template<class T>
inline int Resource<T>::ShowErrorMessage(const char* name, const char* extra)
{
	TCHAR mesg[128];
	wsprintf(mesg, _T("%s「%s」の読込に失敗しました。\n%s"), typeid(T).name()+5, name, extra);
	return MessageBox(Window::GetHWnd(), mesg, "エラー", MB_ABORTRETRYIGNORE | MB_ICONWARNING);
}

template<class T>
inline std::string Resource<T>::GetFullPath(std::string short_path)
{
	return BasePath + (T::BasePath + short_path);
}


template<class T>
inline void Resource<T>::Release(void)
{
	Release(name);
}


