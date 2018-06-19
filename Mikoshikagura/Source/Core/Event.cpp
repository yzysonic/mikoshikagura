#include "Event.h"

void Event::operator()(void)
{
	for (auto callback : this->callback_list)
		callback();

}

void Event::operator=(Func callback)
{
	this->callback_list.clear();
	if(callback)
		*this += callback;
}

void Event::operator+=(Func callback)
{
	this->callback_list.push_back(callback);
}

Event::operator bool()
{
	return !callback_list.empty();
}
