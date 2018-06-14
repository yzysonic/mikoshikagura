#include "FallingSnow.h"
#ifdef _DEBUG
#include "InspectorContentFallingSnow.h"
#endif

FallingSnow::FallingSnow(void)
{
#ifdef _DEBUG
	AddComponent<InspectorExtension>(new InspectorContentFallingSnow);
#endif

	name = "FallingSnow";
	transform.position.z = -15.0f;
	particle = AddComponent<ParticleSystem>(1000);
	particle->SetBehavior(&behavior);
	particle->loop = true;
	particle->pTexture = Texture::Get("particle");
	particle->emission_rate = 100.0f;
}


void SnowParticleBehavior::Init(ParticleElement & element)
{
	element.init_pos.x = Randomf(-70.0f, 70.0f);
	element.transform.position.y = 50.0f + Randomf(0.0f, 10.0f);
	element.random_seed = Randomf(0.0f, 10.0f);
}

void SnowParticleBehavior::Update(ParticleElement & element)
{
	element.transform.position.y -= falling_speed*Time::DeltaTime();
	element.transform.position.x = element.init_pos.x+noise_scale*PerlinNoise(element.random_seed+element.transform.position.y*noise_frequency, noise_octavers);

	if (element.transform.position.y < -30.0f)
		element.active = false;
}
