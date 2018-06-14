#pragma once
#include "Script.h"
#include "Sound.h"
#include "Time.h"
#include <functional>

class SoundPlayer : public Script
{
public:
	bool play_on_active;

	SoundPlayer(const char* name, bool play_on_active = false);
	SoundPlayer(Sound* sound, bool play_on_active = false);

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void SetSound(const char* name);
	void SetSound(Sound* sound);
	void SetVolume(float value);
	void Play(void);
	void Stop(void);
	void FadeVolume(float target_value, float time = 1.0f, std::function<void(void)> callback = nullptr);
	void FadeIn(float time);
	void FadeOut(float time);


private:
	Sound * pSound;
	std::string sound_name;
	float volume;
	FrameTimer timer;
	std::function<void(void)> update;

};