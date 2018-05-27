#include "SeasonObject.h"
#include "SeasonManager.h"

SeasonObject::SeasonObject(void)
{
	state = SeasonType::None;
	SeasonManager::AddObject(this);
}

SeasonObject::~SeasonObject(void)
{
	SeasonManager::RemoveObject(this);
}