#pragma once
#include "Core\Core.h"

class FallingSnow : public Object
{
public:
	FallingSnow(void);

private:
	ParticleSystem* particle;
};

class SnowParticleBehavior : public IParticleBehavior
{
public:
	void Init(ParticleElement & element) override;
	void Update(ParticleElement & element) override;
};