#include "SeasonSoundPlayer.h"
#include "SeasonManager.h"

SeasonSoundPlayer::SeasonSoundPlayer(std::string sound_name)
{
	name = "FieldBgmPlayer_" + sound_name;

	auto sound_summer = Sound::Get(sound_name + "_summer");
	auto sound_winter = Sound::Get(sound_name + "_winter");
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

void SeasonSoundPlayer::SetSummer(void)
{
	player_summer->FadeVolume(1.0f);
	player_winter->FadeVolume(0.0f);
}

void SeasonSoundPlayer::SetWinter(void)
{
	player_summer->FadeVolume(0.0f);
	player_winter->FadeVolume(1.0f);
}