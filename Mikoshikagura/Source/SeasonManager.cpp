#include "SeasonManager.h"

void SeasonManager::SetSeason(SeasonType season)
{
	if (season == SeasonType::Summer)
	{
		for (auto obj : m_pInstance->object_list)
		{
			obj->SetSummer();
		}
	}
	else if (season == SeasonType::Winter)
	{
		for (auto obj : m_pInstance->object_list)
		{
			obj->SetWinter();
		}
	}
	
	m_pInstance->current_season = season;
}

SeasonType SeasonManager::GetSeason(void)
{
	return m_pInstance->current_season;
}

void SeasonManager::AddObject(SeasonObject * obj)
{
	if (!m_pInstance)
		return;

	m_pInstance->object_list.emplace_back(obj);
}

void SeasonManager::RemoveObject(SeasonObject * obj)
{
	if (!m_pInstance)
		return;

	auto& list = m_pInstance->object_list;
	for (auto it = list.begin(); it != list.end(); it++)
	{
		if (*it == obj)
		{
			list.erase(it);
			break;
		}
	}
}