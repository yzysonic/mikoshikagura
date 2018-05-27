#pragma once
#include "Core/Core.h"
#include "SeasonObject.h"

class SeasonManager : public Singleton<SeasonManager>
{
	friend class Singleton<SeasonManager>;

public:
	static void SetSeason(SeasonType season);
	static SeasonType GetSeason(void);
	static void AddObject(SeasonObject* obj);
	static void RemoveObject(SeasonObject* obj);

private:
	SeasonType current_season;
	std::vector<SeasonObject*>object_list;
	SeasonManager(void) {};
};