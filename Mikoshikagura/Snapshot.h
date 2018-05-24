#pragma once
#include "Core\Core.h"

class Snapshot : public Script
{
public:
	FrameTimer timer;
	std::string fileName;

	Snapshot(void) { active_on_load = false; }
	void Init(void) override;
	void Update(void) override;
	
private:
	int state;
};