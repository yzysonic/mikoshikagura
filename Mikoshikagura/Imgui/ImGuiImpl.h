#pragma once

#include "imgui.h"
// DirectX
#include "../Core/Singleton.h"
#include "../Core/Direct3D.h"

class ImGuiImpl : public Singleton<ImGuiImpl>, public ILostAndReset
{
private:
	struct CUSTOMVERTEX
	{
		float    pos[3];
		D3DCOLOR col;
		float    uv[2];
	};

	// Win32 data
	HWND					m_hWnd;
	INT64					m_time;
	INT64					m_ticksPerSecond;
	ImGuiMouseCursor		m_lastMouseCursor;

	// DirectX data
	LPDIRECT3DDEVICE9		m_device;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DTEXTURE9		m_fontTexture;
	int						m_vertexBufferSize;
	int						m_indexBufferSize;

public:
	static void Create(void * hwnd, IDirect3DDevice9 * device);
	static void Destroy(void);
	static void NewFrame(void);
	static void EndFrame(void);
	static void Draw(void);
	static LRESULT WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HRESULT OnLostDevice(void) override;
	HRESULT OnResetDevice(void) override;


private:
	friend Singleton<ImGuiImpl>;
	ImGuiImpl(void);

	static bool UpdateMouseCursor(void);

	IMGUI_API bool Init(void* hwnd, IDirect3DDevice9* device);
	IMGUI_API void Uninit(void);
	IMGUI_API void _NewFrame(void);
	IMGUI_API void _EndFrame(void);
	IMGUI_API void _Draw(void);

	bool CreateFontsTexture(void);
};