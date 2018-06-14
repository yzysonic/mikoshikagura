#pragma once
#include "Core/Core.h"
#include "ISeason.h"

class SeasonManager : public Singleton<SeasonManager>
{
	friend class Singleton<SeasonManager>;

public:
	static void Create(SeasonType season = SeasonType::Summer);
	static void SetSeason(SeasonType season, Action callback = nullptr);
	static void SwitchSeason(Action callback = nullptr);
	static SeasonType GetSeason(void);
	static void AddObject(ISeason* obj);
	static void RemoveObject(ISeason* obj);

private:
	static void InternalSetSeason(SeasonType season);

	SeasonType current_season;
	std::vector<ISeason*>object_list;
	SeasonManager(void) : current_season(SeasonType::None) {};
};