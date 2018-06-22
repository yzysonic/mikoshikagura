#include "FallingSnow.h"
#include "SeasonManager.h"
#ifdef _DEBUG
#include "InspectorContentFallingSnow.h"
#endif

FallingSnow::FallingSnow(MapManager* map) :behavior(map)
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
	particle->emission_rate = 50.0f;

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


SnowParticleBehavior::SnowParticleBehavior(MapManager* map)
{
	camera = &RenderSpace::Get("default")->GetCamera(0)->transform;
	this->map = map;
}

void SnowParticleBehavior::Init(ParticleElement & element)
{
	element.init_pos.x = camera->position.x + Randomf(-camera_range, camera_range);
	element.transform.position.y = camera->position.y + 40.f + Randomf(0.0f, 10.0f);
	element.transform.position.z = Randomf(-30.f, 30.f);
	element.random_seed = Randomf(0.0f, 10.0f);
	element.color = Color(255, 255, 255, 255);
	element.timer.Reset(0.f);
}

void SnowParticleBehavior::Update(ParticleElement & element)
{
	if (element.timer.interval == 0.f)
	{
		element.transform.position.y -= falling_speed*Time::DeltaTime();
		element.transform.position.x = element.init_pos.x + noise_scale*PerlinNoise(element.random_seed + element.transform.position.y*noise_frequency, noise_octavers);

		if (element.transform.position.x > camera->position.x + camera_range)
			element.transform.position.x -= 2.0f*camera_range;
		if (element.transform.position.x < camera->position.x - camera_range)
			element.transform.position.x += 2.0f*camera_range;

		if (element.transform.position.y < camera->position.y - 100.0f)
		{
			element.transform.position.y = camera->position.y + 50.f + Randomf(0.0f, 10.0f);
			element.transform.position.z = Randomf(-30.f, 30.f);
		}

		if (element.transform.position.z < 5.f && element.transform.position.z > -5.f
			&& map->GetGroundPosition(element.transform.position.x) > element.transform.position.y)
			element.timer.Reset(1.0f);
		else if(element.transform.position.z <= -5.f && map->GetGroundPosition(element.transform.position.x) > element.transform.position.y + element.transform.position.z * .05f)
			element.timer.Reset(.5f);
	}
	else
	{
		if (element.transform.position.z < -5.f)
		{
			element.transform.position.y -= falling_speed*Time::DeltaTime();
			element.transform.position.x = element.init_pos.x + noise_scale*PerlinNoise(element.random_seed + element.transform.position.y*noise_frequency, noise_octavers);
		}

		if (element.transform.position.x > camera->position.x + camera_range)
			element.transform.position.x -= 2.0f*camera_range;
		if (element.transform.position.x < camera->position.x - camera_range)
			element.transform.position.x += 2.0f*camera_range;

		element.color = Color(255, 255, 255, (int)(255 * (1 - element.timer.Progress())));
		element.timer++;

		if (element.timer.TimeUp())
			element.active = false;
	}
}
