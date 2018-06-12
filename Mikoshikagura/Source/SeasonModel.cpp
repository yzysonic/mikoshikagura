#include "SeasonModel.h"
#include "SeasonManager.h"

SeasonModel::SeasonModel(const char * model_name, bool auto_change) : StaticModel(""), ISeason(auto_change)
{
	active_on_load = true;
	data_summer = ModelData::Get(model_name + std::string("_summer"));
	data_winter = ModelData::Get(model_name + std::string("_winter"));
	SetSeason(SeasonManager::GetSeason());
}

void SeasonModel::SetSummer(void)
{
	pData = data_summer;
	current_season = SeasonType::Summer;
}

void SeasonModel::SetWinter(void)
{
	pData = data_winter;
	current_season = SeasonType::Winter;
}

void SeasonModel::SetSeason(SeasonType season)
{
	switch (season)
	{
	case SeasonType::Summer:
		SetSummer();
		break;

	case SeasonType::Winter:
		SetWinter();
		break;

	default:
		active_on_load = false;
		current_season = season;
	}
}

SeasonType SeasonModel::GetSeason(void)
{
	return current_season;
}
