#pragma once
#include "Core\Core.h"
#include "ISeason.h"
#include "MapManager.h"

class SnowParticleBehavior : public IParticleBehavior
{
public:
	int		noise_octavers	= 2;
	float	falling_speed	= 10.0f;
	float	noise_scale		= 9.0f;
	float	noise_frequency = 0.05f;
	float	camera_range	= 100.0f;

	SnowParticleBehavior(MapManager* map);
	void Init(ParticleElement & element) override;
	void Update(ParticleElement & element) override;

private:
	Transform* camera;
	MapManager* map;
};

class FallingSnow : public Object, public ISeason
{
public:
	SnowParticleBehavior behavior;
	FallingSnow(MapManager* map);
	void SetSummer(void) override;
	void SetWinter(void) override;

private:
	ParticleSystem* particle;
};