#pragma once
#include "Core\Core.h"

class DebugManager : public Object, public Singleton<DebugManager>
{
public:
	static void TakeSnapshot(const char* fileName, float waitTime);
	void Update(void) override;

private:
	friend Singleton<DebugManager>;
	DebugManager(void);
};