#include "Sign.h"

Sign::Sign(std::string message, Hukidashi * hukidashi)
{
	this->message = message;
	this->hukidashi = hukidashi;
}

void Sign::Update(void)
{
}

void Sign::OnCollisionEnter(Object * object)
{
	if(object->type == ObjectType::Player)
		this->hukidashi->Pop(this->message);
}

void Sign::OnCollisionExit(Object * object)
{
	if (object->type == ObjectType::Player)
		this->hukidashi->Unpop(this->message);
}

void Sign::SetText(std::string message, Hukidashi * hukidashi)
{
	this->message = message;
	this->hukidashi = hukidashi;
}
