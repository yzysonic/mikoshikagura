#include "Script.h"

Script::Script(void)
{
	this->type = ComponentType::Script;
}

bool Script::SetActive(bool value)
{
	if (!this->Component::SetActive(value))
		return false;

	if (value) 
		Init();
	else 
		Uninit();

	return true;
}
