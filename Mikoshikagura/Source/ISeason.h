#pragma once
#include "Core/Core.h"

enum class SeasonType
{
	None,
	Summer,
	Winter
};

class ISeason
{
public:
	ISeason(bool auto_change = true);
	~ISeason(void);
	virtual void SetSummer(void) = 0;
	virtual void SetWinter(void) = 0;

protected:
	SeasonType state;
	bool auto_change;
};