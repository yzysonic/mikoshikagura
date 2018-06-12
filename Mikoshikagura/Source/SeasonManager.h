#pragma once
#include "Core/Core.h"
#include "ISeason.h"

class SeasonManager : public Singleton<SeasonManager>
{
	friend class Singleton<SeasonManager>;

public:
	static void SetSeason(SeasonType season);
	static SeasonType GetSeason(void);
	static void AddObject(ISeason* obj);
	static void RemoveObject(ISeason* obj);

private:
	SeasonType current_season;
	std::vector<ISeason*>object_list;
	SeasonManager(void) : current_season(SeasonType::None) {};
};