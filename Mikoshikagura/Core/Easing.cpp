//�Q�l�Fhttp://gizma.com/easing/

#include "Easing.h"
#include "Math.h"

float Ease::CircIn(float a, float b, float t)
{
	return -(b-a) * (sqrtf(1 - t*t) - 1) + a;
}

float Ease::CircOut(float a, float b, float t)
{
	t--;
	return (b-a) * sqrtf(1 - t*t) + a;
}

float Ease::CircInOut(float a, float b, float t)
{
	t *= 2.0f;
	if (t < 1) return -(b-a) / 2 * (sqrtf(1 - t*t) - 1) + a;
	t -= 2;
	return (b-a) / 2 * (sqrtf(1 - t*t) + 1) + a;
}

float Ease::CubicIn(float a, float b, float t)
{
	return (b - a)*t*t*t*t + a;
}

float Ease::CubicOut(float a, float b, float t)
{
	t--;
	return (b-a)*(t*t*t + 1) + a;

}

float Ease::CubicInOut(float a, float b, float t)
{
	t *= 2.0f;
	if (t < 1) return (b-a) / 2 * t*t*t + a;
	t -= 2;
	return (b-a) / 2 * (t*t*t + 2) + a;
}

float Ease::QuadIn(float a, float b, float t)
{
	return (b-a)*t*t + a;
}

float Ease::QuadOut(float a, float b, float t)
{
	return (a-b) * t*(t - 2) + a;
}

float Ease::QuadInOut(float a, float b, float t)
{
	t *= 2.0f;
	if (t < 1) return (b-a) / 2 * t*t + a;
	t--;
	return (a-b) / 2 * (t*(t - 2) - 1) + a;
}

float Ease::ExpoIn(float a, float b, float t)
{
	return (b-a) * powf( 2, 10 * (t - 1) ) + a;
}

float Ease::ExpoOut(float a, float b, float t)
{
	return (b-a) * (-powf(2, -10 * t) + 1) + a;
}

float Ease::ExpoInOut(float a, float b, float t)
{
	t *= 2.0f;
	if (t < 1) return (b-a) / 2 * powf(2, 10 * (t - 1)) + a;
	t--;
	return (b-a) / 2 * (-powf(2, -10 * t) + 2) + a;
}
