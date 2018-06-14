#include "SeasonManager.h"
#include "FadeScreen.h"

void SeasonManager::Create(SeasonType season)
{
	Singleton<SeasonManager>::Create();
	InternalSetSeason(season);
}

void SeasonManager::SetSeason(SeasonType season, Action callback)
{
	FadeScreen::FadeOut(Color::white, 0.3f, [&, season, callback]
	{
		InternalSetSeason(season);
		if (callback) callback();
		FadeScreen::FadeIn(Color::white);
	});
}

void SeasonManager::SwitchSeason(Action callback)
{
	switch (m_pInstance->current_season)
	{
	case SeasonType::Summer:
		SetSeason(SeasonType::Winter, callback);
		break;

	case SeasonType::Winter:
		SetSeason(SeasonType::Summer, callback);
	}
}

SeasonType SeasonManager::GetSeason(void)
{
	return m_pInstance->current_season;
}

void SeasonManager::AddObject(ISeason * obj)
{
	if (!m_pInstance)
		return;

	m_pInstance->object_list.emplace_back(obj);
}

void SeasonManager::RemoveObject(ISeason * obj)
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

void SeasonManager::InternalSetSeason(SeasonType season)
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
