#pragma once

#include <functional>
#include <vector>

class Event
{
	using Func = std::function<void(void)>;

public:
	void operator ()(void);
	void operator =(Func callback);
	void operator +=(Func callback);
	operator bool();

private:
	std::vector<Func> callback_list;
};
