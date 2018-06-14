#pragma once
#include "Core\Core.h"

class SnowParticleBehavior : public IParticleBehavior
{
public:
	int		noise_octavers	= 2;
	float	falling_speed	= 10.0f;
	float	noise_scale		= 9.0f;
	float	noise_frequency = 0.05f;

	void Init(ParticleElement & element) override;
	void Update(ParticleElement & element) override;
};

class FallingSnow : public Object
{
public:
	SnowParticleBehavior behavior;
	FallingSnow(void);

private:
	ParticleSystem* particle;
};