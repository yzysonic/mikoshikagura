#pragma once
#include "Core/Core.h"
#include "Hukidashi.h"

class Sign : public Object
{
public:
	Sign(std::string message, Hukidashi *hukidashi);
	void Update(void) override;
	void OnCollisionStay(Object *object) override;

private:
	int delay;
	Hukidashi *hukidashi;
	std::string message;
};