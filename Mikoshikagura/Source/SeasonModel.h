#pragma once
#include "Core\Core.h"
#include "ISeason.h"

class SeasonModel : public StaticModel, public ISeason
{
public:
	SeasonModel(const char* model_name, bool auto_change = true);
	void SetSummer(void) override;
	void SetWinter(void) override;
	void SetSeason(SeasonType season);
	SeasonType GetSeason(void);

private:
	ModelData* data_summer;
	ModelData* data_winter;
	SeasonType current_season;
};