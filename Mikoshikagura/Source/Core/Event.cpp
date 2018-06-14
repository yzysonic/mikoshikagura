#include "Event.h"

void Event::operator()(void)
{
	for (auto callback : this->callback_list)
		callback();

}

void Event::operator=(std::function<void(void)> callback)
{
	this->callback_list.clear();
	*this += callback;
}

void Event::operator+=(std::function<void(void)> callback)
{
	this->callback_list.push_back(callback);
}

Event::operator bool(void)
{
	return !callback_list.empty();
}
