#pragma once

#include <dsound.h>
#include <windows.h>
#include "Resource.h"

class System;

class Sound : public Resource<Sound>
{
	friend class Resource<Sound>;
	friend class System;

public:
	static constexpr char* BasePath = "Sound/";
	static constexpr char* DefaultExtension = ".wav";
	static constexpr long MinVolume = DSBVOLUME_MIN;
	static void SetGlobalVolume(float value);
	static float GetGlobalVolume(void);

private:
	static float global_volume;
	static IDirectSound8 *pDirectSound; // サウンドインターフェース
	static Sound* InternalLoad(std::string name, std::string ext);
	static HRESULT Init(HWND hWnd);
	static void Uninit(void);

public:
	LPDIRECTSOUNDBUFFER8 pBuffer;
	bool loop;

	Sound(std::string name);
	~Sound(void);

	void Play(void);
	void Stop(void);
	void SetVolume(float value);
	float GetVolume(void);
	bool IsPlaying(void);
	DWORD GetCurrentPosition(void);

private:
	float volume;
};