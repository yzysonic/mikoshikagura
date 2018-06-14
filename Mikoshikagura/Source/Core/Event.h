#pragma once

#include <functional>
#include <vector>

class Event
{
public:

	void operator ()(void);

	void operator =(std::function<void(void)> callback);
	void operator +=(std::function<void(void)> callback);
	operator bool(void);
private:
	std::vector<std::function<void(void)>> callback_list;
};
