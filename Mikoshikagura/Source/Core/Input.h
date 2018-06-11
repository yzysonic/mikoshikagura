//=============================================================================
//
// �t�@�C�����Finput.h
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include <dinput.h>
#include <windows.h>
#include <tchar.h>
#include "Vector.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************


// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD										// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE											// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD												// �錾����ƃp�b�h�ő���\�ɂȂ�


/* game pad��� */
#define BUTTON_UP		0x00000001l	// �����L�[��
#define BUTTON_DOWN		0x00000002l	// �����L�[��
#define BUTTON_LEFT		0x00000004l	// �����L�[��
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E
#define BUTTON_SQ		0x00000010l	// ���{�^��(.rgbButtons[0]&0x80)
#define BUTTON_CR		0x00000020l	// �~�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_CI		0x00000040l	// ���{�^��(.rgbButtons[2]&0x80)
#define BUTTON_TR		0x00000080l	// ���{�^��(.rgbButtons[3]&0x80)
#define BUTTON_L1		0x00000100l	// L1�{�^��(.rgbButtons[4]&0x80)
#define BUTTON_R1		0x00000200l	// R1�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L2		0x00000400l	// L2�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R2		0x00000800l	// R2�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_SHARE	0x00001000l	// SHARE�{�^��(.rgbButtons[8]&0x80)
#define BUTTON_OPTIONS	0x00002000l	// OPTIONS�{�^��(.rgbButtons[9]&0x80)
#define BUTTON_L3		0x00004000l	// L3�{�^��(.rgbButtons[10]&0x80)
#define BUTTON_R3		0x00008000l	// R3�{�^��(.rgbButtons[11]&0x80)
#define BUTTON_PS		0x00010000l	// PS�{�^��(.rgbButtons[12]&0x80)
#define BUTTON_PAD		0x00020000l	// PAD�{�^��(.rgbButtons[13]&0x80)
#define GAMEPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g

enum ControlType
{
	kKeyboard = 0x01,
	kMouse = 0x02,
	kPad = 0x04
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardAnyKeyTrigger();

//---------------------------- mouse
BOOL IsMouseLeftPressed(void);      // ���N���b�N�������
BOOL IsMouseLeftTriggered(void);    // ���N���b�N�����u��
BOOL IsMouseRightPressed(void);     // �E�N���b�N�������
BOOL IsMouseRightTriggered(void);   // �E�N���b�N�����u��
BOOL IsMouseCenterPressed(void);    // ���N���b�N�������
BOOL IsMouseCenterTriggered(void);  // ���N���b�N�����u��
BOOL IsMouseMoved(void);
long GetMouseMoveX(void);           // �}�E�X��X�����ɓ��������Βl
long GetMouseMoveY(void);           // �}�E�X��Y�����ɓ��������Βl
long GetMouseMoveZ(void);           // �}�E�X�z�C�[�������������Βl
Vector3 GetMousePos(void);          // �}�E�X�̍��W

//---------------------------- game pad
BOOL IsButtonPressed(DWORD button, int padNo = 0);
BOOL IsButtonTriggered(DWORD button, int padNo = 0);
BOOL IsAnyButtonTriggered(int padNo = 0);
float GetPadLX(void);
float GetPadLY(void);
float GetPadRX(void);
float GetPadRY(void);


#endif