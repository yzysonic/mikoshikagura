#include "System.h"
#include "Direct3D.h"
#include "Window.h"
#include "Input.h"
#include "Sound.h"

HRESULT System::Init()
{
	HRESULT hr;

	// �E�B���h�E�̏�����
	hr = Window::Init();

	// �O���t�B�b�N�X�̏�����
	if (SUCCEEDED(hr))
		hr = Direct3D::Init(Window::GetHWnd(), Window::GetWindowMode());

	// �C���v�b�g�̏�����
	if (SUCCEEDED(hr))
		hr = InitInput(Window::GetHInstance(), Window::GetHWnd());

	// �T�E���h�̏�����
	if (SUCCEEDED(hr))
		hr = Sound::Init(Window::GetHWnd());

	return hr;
}

void System::Uninit()
{
	Sound::Uninit();
	UninitInput();
	Direct3D::Uninit();
	Window::Uninit();
}