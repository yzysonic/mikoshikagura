#include "Window.h"
#include "Common.h"
#include "Direct3D.h"
#include "Game.h"

#ifdef _DEBUG
#include "../Imgui/ImGuiImpl.h"
#endif

HINSTANCE Window::s_hInstance = NULL;
HWND Window::s_hWnd = NULL;
MSG  Window::s_Msg;
bool Window::s_bWindowMode = true;
bool Window::s_bBigMode = false;
char* Window::s_ClassName = "MainWindow";

//=============================================================================
// Windows�̏���������
//=============================================================================
HRESULT Window::Init()
{
	s_hInstance = GetModuleHandle(NULL);
	int width;
	int height;

	if (s_bBigMode)
	{
		width = 1600;
		height = 900;
	}
	else
	{
		width = SystemParameters::ResolutionX;
		height = SystemParameters::ResolutionY;
	}

	// �E�B���h�E�N���X��o�^����
	WNDCLASSEX	wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_CLASSDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = s_hInstance;
	wcex.lpszClassName = s_ClassName;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClassEx(&wcex);


	// �E�B���h�E�I�u�W�F�N�g���쐬����.
	s_hWnd = CreateWindowEx(
		0,							//�g���X�^�C���i�C�Ӂj
		s_ClassName,			//�N���X��
		SystemParameters::windowTitle,				//�E�B���h�E���i�^�C�g���j
		(WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU),		//�X�^�C��
		((GetSystemMetrics(SM_CXSCREEN) - width) / 2),		//�������̈ʒu
		((GetSystemMetrics(SM_CYSCREEN) - height) / 2),	//�c�����̈ʒu
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,		//��
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	//����
		NULL,						//�e�E�B���h�E
		NULL,						//���j���[
		s_hInstance,				//�A�v���P�[�V�����C���X�^���X�̃n���h��
		NULL						//�E�B���h�E�쐬�f�[�^
	);

	if (s_hWnd == NULL)
		return E_FAIL;

	//�E�B���h�E���X�N���[���ɕ\������
	ShowWindow(s_hWnd, SW_SHOW);
	UpdateWindow(s_hWnd);

	return S_OK;
}


//=============================================================================
// Windows�̏I������
//=============================================================================
void Window::Uninit()
{
	// �E�B���h�E�̃f�X�g���C�h
	Destroy();

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(s_ClassName, s_hInstance);
	s_hInstance = NULL;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef IMGUI
	if (ImGuiImpl::WndProcHandler(hWnd, message, wParam, lParam))
		return true;
#endif

	switch (message)
	{
	case WM_CLOSE:
		Game::EndPrompt();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:		// [ESC]�L�[�������ꂽ
			Game::EndPrompt();
			break;
		case VK_F9:			// [F9]�L�[�������ꂽ
			SetWindowMode(!s_bWindowMode);
			break;
		case VK_F8:			// [F9]�L�[�������ꂽ
			if (s_bBigMode)
				SetWindowSize(SystemParameters::ResolutionX, SystemParameters::ResolutionY);
			else
				SetWindowSize(1600, 900);

			s_bBigMode = !s_bBigMode;
			break;

		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Window::CheckMesg(void)
{
	if (PeekMessage(&s_Msg, NULL, 0, 0, PM_REMOVE))
	{
		// PostQuitMessage()���Ă΂ꂽ��Q�[���I��
		if (s_Msg.message == WM_QUIT)
		{
			Game::Stop();
			return;
		}
		// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
		else
		{
			TranslateMessage(&s_Msg);
			DispatchMessage(&s_Msg);
		}
	}

}


HWND Window::GetHWnd()
{
	return s_hWnd;
}

HINSTANCE Window::GetHInstance(void)
{
	return s_hInstance;
}

bool Window::GetWindowMode(void)
{
	return s_bWindowMode;
}

void Window::SetWindowMode(bool windowMode)
{
	if (!Direct3D::SetWindowMode(windowMode))
		return;

	if (windowMode)
	{
		//SetWindowLong(s_hWnd, GWL_STYLE, WS_CAPTION | WS_THICKFRAME | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU);
		SetWindowSize(SystemParameters::ResolutionX, SystemParameters::ResolutionY);
	}
	//else
	//{
	//	SetWindowLong(s_hWnd, GWL_STYLE, WS_POPUP | WS_EX_TOPMOST);
	//}


	//ShowWindow(s_hWnd, SW_SHOW);

	s_bWindowMode = windowMode;
}

void Window::SetWindowSize(int x, int y)
{
	SetWindowPos(s_hWnd, NULL,
		/*(GetSystemMetrics(SM_CXSCREEN) - x) / 2*/0,
		/*(GetSystemMetrics(SM_CYSCREEN) - y) / 2*/0,
		x + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		y + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		SWP_NOZORDER | SWP_SHOWWINDOW);
}

void Window::Destroy(void)
{
	if (s_hWnd == NULL)
		return;

	DestroyWindow(s_hWnd);
	s_hWnd = NULL;
}
