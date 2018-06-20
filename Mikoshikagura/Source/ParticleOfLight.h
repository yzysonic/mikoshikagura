#pragma once
#include "Core\Core.h"
#include "ISeason.h"

class LightParticleBehavior : public IParticleBehavior
{
public:
	int		noise_octavers = 2;
	float	rise_speed = 5.0f;
	float	noise_scale = 9.0f;
	float	noise_frequency = 0.05f;
	float	camera_range = 100.0f;

	LightParticleBehavior(void);
	void Init(ParticleElement & element) override;
	void Update(ParticleElement & element) override;

private:
	Transform* camera;
};

class ParticleOfLight : public Object, public ISeason
{
public:
	LightParticleBehavior behavior;
	ParticleOfLight(void);
	void OnDraw() override;
	void AfterDraw() override;
	void SetSummer(void);
	void SetWinter(void);

private:
	ParticleSystem* particle;
};