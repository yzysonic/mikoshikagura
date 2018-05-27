//=============================================================================
//
// ファイル名：common.h
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#pragma once

#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <memory>

//システムのパラメータ
//数値の変更に反映するのはシステム初期化するときのみ。
struct SystemParameters
{
	static char* windowTitle;	// ウィンドウタイトル
	static int ResolutionX;		// 画面の解像度X
	static int ResolutionY;		// 画面の解像度Y
	static int FPS;				// 最大FPS
	static int SystemParameters::ObjectMax;		// 最大オブジェクト数
};

template<class T>
void SafeDelete(T *&ptr)
{
	delete ptr;
	ptr = nullptr;
}

template<class T>
void SafeRelease(T *&ptr)
{
	if (ptr == nullptr) return;
	ptr->Release();
	ptr = nullptr;
}

template<class T>
void SafeFree(T *&ptr)
{
	free(ptr);
	ptr = nullptr;
}

inline std::string TypeName(const std::type_info& type)
{
	auto char_name = type.name();
	auto name = std::string(char_name);

	if (name.find("class") != std::string::npos)
		char_name += 6;
	else if (name.find("struct") != std::string::npos)
		char_name += 7;

	return std::string(char_name);

}

template<class T>
std::string TypeName(void)
{
	return TypeName(typeid(T));
}

#define ColorRGBA(r,g,b,a)		D3DCOLOR_RGBA(r,g,b,a)


template<class T>
using smart_ptr = std::unique_ptr<T>;