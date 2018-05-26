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

#define ColorRGBA(r,g,b,a)		D3DCOLOR_RGBA(r,g,b,a)


template<class T>
using smart_ptr = std::unique_ptr<T>;