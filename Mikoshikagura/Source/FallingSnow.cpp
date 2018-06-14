#include "FallingSnow.h"
#include "SeasonManager.h"
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
	particle->SetLayer(Layer::MASK);
	particle->SetBehavior(&behavior);
	particle->loop = true;
	particle->pTexture = Texture::Get("particle");
	particle->emission_rate = 100.0f;

	if (SeasonManager::GetSeason() == SeasonType::Summer)
		SetActive(false);
}

void FallingSnow::SetSummer(void)
{
	SetActive(false);
}

void FallingSnow::SetWinter(void)
{
	SetActive(true);
}


SnowParticleBehavior::SnowParticleBehavior(void)
{
	camera = &RenderSpace::Get("default")->GetCamera(0)->transform;
}

void SnowParticleBehavior::Init(ParticleElement & element)
{
	element.init_pos.x = camera->position.x + Randomf(-camera_range, camera_range);
	element.transform.position.y = camera->position.y + 30.f + Randomf(0.0f, 10.0f);
	element.transform.position.z = Randomf(-30.f, 30.f);
	element.random_seed = Randomf(0.0f, 10.0f);
}

void SnowParticleBehavior::Update(ParticleElement & element)
{
	element.transform.position.y -= falling_speed*Time::DeltaTime();
	element.transform.position.x = element.init_pos.x+noise_scale*PerlinNoise(element.random_seed+element.transform.position.y*noise_frequency, noise_octavers);

	if (element.transform.position.x > camera->position.x + camera_range)
		element.transform.position.x -= 2.0*camera_range;
	if (element.transform.position.x < camera->position.x - camera_range)
		element.transform.position.x += 2.0*camera_range;

	if (element.transform.position.y < 0.0f)
		element.active = false;
}
