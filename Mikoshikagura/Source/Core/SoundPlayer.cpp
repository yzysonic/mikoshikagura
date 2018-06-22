#include "SoundPlayer.h"
#include "Object.h"
#include "Math.h"

SoundPlayer::SoundPlayer(const char * name, bool play_on_active) : SoundPlayer(Sound::Get(name), play_on_active){}

SoundPlayer::SoundPlayer(Sound * pSound, bool play_on_active)
{
	this->play_on_active = false;
	this->volume = 1.0f;

	SetSound(pSound);
}

void SoundPlayer::Init(void)
{
	if (this->play_on_active)
		Play();
}

void SoundPlayer::Update(void)
{
	if (update)
	{
		update();
	}
}

void SoundPlayer::Uninit(void)
{
	if (Sound::Get(sound_name))
		Stop();
}

void SoundPlayer::SetSound(const char * name)
{
	SetSound(Sound::Get(name));
}

void SoundPlayer::SetSound(Sound * sound)
{
	this->pSound = sound;
	
	if (!sound)
		return;

	this->sound_name = sound->name;
	sound->SetVolume(this->volume);
}

void SoundPlayer::SetVolume(float value)
{
	this->volume = value;
	if (this->pSound)
	{
		this->pSound->SetVolume(value);
	}
}

void SoundPlayer::Play(void)
{
	if (this->pSound)
	{
		this->pSound->Play();
	}
}

void SoundPlayer::Stop(void)
{
	if (this->pSound)
	{
		this->pSound->Stop();
	}
}

void SoundPlayer::FadeVolume(float target_value, float time, std::function<void(void)> callback)
{
	auto current_value = this->pSound->GetVolume();
	this->timer.Reset(time);

	this->update = [current_value, target_value, callback, this]
	{
		this->timer++;

		if (this->pSound)
		{
			this->pSound->SetVolume(Lerpf(current_value, target_value, timer.Progress()));
		}

		if (this->timer.TimeUp())
		{
			if(callback != nullptr)
				callback();
			this->update = nullptr;
		}
	};
}

void SoundPlayer::FadeIn(float time)
{
	this->pSound->SetVolume(0.0f);
	Play();
	FadeVolume(1.0f, time);
}

void SoundPlayer::FadeOut(float time)
{
	this->pSound->SetVolume(1.0f);
	FadeVolume(0.0f, time, [this] {Stop(); });
}
