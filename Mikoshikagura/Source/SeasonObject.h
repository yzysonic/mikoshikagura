#pragma once
#include "Core/Core.h"

enum class SeasonType
{
	None,
	Summer,
	Winter
};

class SeasonObject : public Object
{
public:

	SeasonObject(void);
	~SeasonObject(void);
	virtual void SetSummer(void) = 0;
	virtual void SetWinter(void) = 0;

protected:
	SeasonType state;
};