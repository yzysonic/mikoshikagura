#pragma once
#include "Core/Core.h"
#include "ISeason.h"

class FieldBgmPlayer : public Object, public ISeason
{
public:
	FieldBgmPlayer(void);
	void SetSummer(void);
	void SetWinter(void);

private:
	SoundPlayer * player_summer;
	SoundPlayer * player_winter;
};