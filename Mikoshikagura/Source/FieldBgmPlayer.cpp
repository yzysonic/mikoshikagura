#include "FieldBgmPlayer.h"
#include "SeasonManager.h"

FieldBgmPlayer::FieldBgmPlayer(void)
{
	name = "FieldBgmPlayer";

	auto sound_summer = Sound::Get("field_summer");
	auto sound_winter = Sound::Get("field_winter");
	sound_summer->loop = true;
	sound_winter->loop = true;

	player_summer = AddComponent<SoundPlayer>(sound_summer, true);
	player_winter = AddComponent<SoundPlayer>(sound_winter, true);
	
	if (SeasonManager::GetSeason() == SeasonType::Summer)
	{
		player_winter->SetVolume(0.0f);
	}
	else
	{
		player_summer->SetVolume(0.0f);
	}
}

void FieldBgmPlayer::SetSummer(void)
{
	player_summer->FadeVolume(1.0f);
	player_winter->FadeVolume(0.0f);
}

void FieldBgmPlayer::SetWinter(void)
{
	player_summer->FadeVolume(0.0f);
	player_winter->FadeVolume(1.0f);
}