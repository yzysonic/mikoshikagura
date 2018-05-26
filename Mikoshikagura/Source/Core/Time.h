#pragma once

#include "Common.h"
#include "Singleton.h"

class Timer
{
public:
	DWORD startTime;
	DWORD interval;

	Timer(float interval = 0.0f);
	void reset(void);
	float elapsedTime(void);
	bool timeUp(void);
};

class FrameTimer
{
public:
	float interval;

	FrameTimer(float interval = 0.0f);
	void Step(void);
	void Reset(void);
	void Reset(float interval);
	float Progress(void);
	float Elapsed(void);
	bool TimeUp(void);

	void operator ++(int);
private:
	float elapsed;
};

class Time
{
public:
	static void Init(void);
	static void Uninit(void);
	static float DeltaTime(void);
	static void FramerateControl();
	static int FPS();

private:
	static Timer* s_pFrameTimer;
	static Timer* s_pFPSTimer;
	static DWORD s_FrameError;
	static DWORD s_FrameCount;
	static float s_DeltaTime;
	static int s_fps;
};