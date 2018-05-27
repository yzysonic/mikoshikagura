#include "Color.h"

const Color Color::white	= Color(255, 255, 255, 255);
const Color Color::black	= Color(0, 0, 0, 255);
const Color Color::red		= Color(255, 0, 0, 255);
const Color Color::green	= Color(0, 255, 0, 255);
const Color Color::blue		= Color(0, 0, 255, 255);
const Color Color::none		= Color(0, 0, 0, 0);


Color::Color(void) : Color(0,0,0,0)
{
}

Color::Color(unsigned long color)
{
	*this = *(Color*)&color;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	setRGBA(r, g, b, a);
}


void Color::setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::operator unsigned long() const
{
	return *(unsigned long*)this;
}



Colorf::Colorf(void) : r(0.0f), g(0.0f), b(0.0f), a(0.0f){}
Colorf::Colorf(float r, float g, float b, float a) : r(r), g(g), b(b), a(a){}

Colorf::operator Color() const
{
	return Color((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), (unsigned char)(a * 255));
}

Colorf::Colorf(Color color)
{
	a = color.a*Color::Divisor;
	r = color.r*Color::Divisor;
	g = color.g*Color::Divisor;
	b = color.b*Color::Divisor;
}

