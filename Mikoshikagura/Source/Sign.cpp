#include "Sign.h"
#define POPDELAY 3

Sign::Sign(std::string message, Hukidashi * hukidashi)
{
	this->message = message;
	this->hukidashi = hukidashi;
	delay = -1;
}

void Sign::Update(void)
{
	if (delay>0)
	{
		delay--;
	}
	else if (delay == 0)
	{
		delay = -1;
		this->hukidashi->Unpop();
	}
}

void Sign::OnCollisionStay(Object * object)
{
	this->hukidashi->Pop(this->message);
	delay = POPDELAY;
}
