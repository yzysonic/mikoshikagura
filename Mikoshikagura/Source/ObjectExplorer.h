#pragma once
#include "Core/Core.h"

class ObjectExplorer : public Script
{
	static constexpr float Width = 300.0f;

public:
	ObjectExplorer(void);
	void Init(void) override;
	void Update(void) override;

private:
	int selected;
	bool active;
};