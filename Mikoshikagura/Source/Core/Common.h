//=============================================================================
//
// �t�@�C�����Fcommon.h
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#pragma once

#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <memory>
#include <functional>

//�V�X�e���̃p�����[�^
//���l�̕ύX�ɔ��f����̂̓V�X�e������������Ƃ��̂݁B
struct SystemParameters
{
	static char* windowTitle;	// �E�B���h�E�^�C�g��
	static int ResolutionX;		// ��ʂ̉𑜓xX
	static int ResolutionY;		// ��ʂ̉𑜓xY
	static int FPS;				// �ő�FPS
	static int SystemParameters::ObjectMax;		// �ő�I�u�W�F�N�g��
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

using Action = std::function<void(void)>;