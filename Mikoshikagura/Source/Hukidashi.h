#pragma once
#include "Core/Core.h"
#include "Core/Text.h"

class Hukidashi : public Object
{
public:
	Hukidashi(void);
	~Hukidashi(void) {};
	void Update(void) override;
	void Pop(std::string message);
	void Unpop(void);
private:
	enum STATE
	{
		none = 0,
		popping,
		popped,
		unpopping
	};
	int pop_count, read_count, str_head;
	std::string message;
	Vector2 hukidashi_pos;
	RECT *area;
	Vector2 text_pos, text_size;
	STATE state;
	float radian;
};