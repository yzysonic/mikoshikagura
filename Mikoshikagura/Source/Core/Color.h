#pragma once

class Color
{
public:
	static constexpr float Divisor = 1 / 255.0f;

public:
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;

	Color(void);
	Color(unsigned long color);
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	
	void setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	operator unsigned long() const;


public:
	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color none;
};

class Colorf
{
public:
	float r;
	float g;
	float b;
	float a;

	Colorf(void);
	Colorf(float r, float g, float b, float a);
	Colorf(Color color);
	
	operator Color() const;
};