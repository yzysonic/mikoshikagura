#pragma once
#include "Core/Core.h"
#include "Hukidashi.h"

class Sign : public Object
{
public:
	Sign();
	Sign(std::string message, Hukidashi *hukidashi);
	void Update(void) override;
	void OnCollisionEnter(Object *object) override;
	void OnCollisionExit(Object *object) override;
	void SetText(std::string message, Hukidashi *hukidashi);

private:
	Hukidashi *hukidashi;
	std::string message;
};