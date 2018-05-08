#ifndef _SOUND_H_
#define _SOUND_H_

#include <dsound.h>
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>

#define SOUND_DIR "Data/Sound/"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
enum SoundName
{	// �T�E���h�ʂ��i���o�[(sound.cpp�̏��Ԃƍ��킹�鎖)
	BGM_TITLE,
	//BGM_GAME,
	//BGM_REST,
	//SE_OK,
	//SE_DAMAGE,
	//SE_ELEMENT,
	SOUND_MAX
};

enum
{	// �Đ��p�t���O
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT					InitSound( HWND hWnd );	// ������
void					UninitSound();			// ��Еt��
LPDIRECTSOUNDBUFFER8	LoadSound( int no );	// �T�E���h�̃��[�h
void					PlaySound( LPDIRECTSOUNDBUFFER8 pBuffer, int flag = 0 );	// �����ƂɍĐ�
bool					IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer );					// �Đ������ǂ���
void					PlayBGM(SoundName sound);
void					PlaySE(SoundName sound);
void					SetVolume(SoundName sound, LONG volume);
void					StopSound(SoundName sound);
void					GetSoundCurrentPos(SoundName sound, LPDWORD playCursor);

#endif