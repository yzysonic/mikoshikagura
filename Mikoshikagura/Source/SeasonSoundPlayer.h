#pragma once
#include "Core/Core.h"
#include "ISeason.h"

class SeasonSoundPlayer : public Object, public ISeason
{
public:
	SeasonSoundPlayer(std::string sound_name);
	void SetSummer(void);
	void SetWinter(void);

private:
	SoundPlayer * player_summer;
	SoundPlayer * player_winter;
};