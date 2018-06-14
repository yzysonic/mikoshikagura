#include "FallingSnow.h"

FallingSnow::FallingSnow(void)
{
	particle = AddComponent<ParticleSystem>(300);
	particle->loop = true;
	particle->pTexture = Texture::Get("particle");
	//particle->SetDuration(0.5f);
	particle->emission_rate = 100.0f;
}


void SnowParticleBehavior::Init(ParticleElement & element)
{

}

void SnowParticleBehavior::Update(ParticleElement & element)
{
}
