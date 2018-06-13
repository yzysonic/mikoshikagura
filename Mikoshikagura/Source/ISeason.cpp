#include "ISeason.h"
#include "SeasonManager.h"

ISeason::ISeason(bool auto_change)
{
	state = SeasonType::None;

	if (auto_change)
	{
		SeasonManager::AddObject(this);
	}
	
	this->auto_change = auto_change;
}

ISeason::~ISeason(void)
{
	if (auto_change)
	{
		SeasonManager::RemoveObject(this);
	}
}