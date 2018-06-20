#include "ParticleOfLight.h"
#include "SeasonManager.h"
#ifdef _DEBUG
#include "InspectorContentParticleOfLight.h"
#endif

ParticleOfLight::ParticleOfLight(MapManager *map) :behavior(map)
{
#ifdef _DEBUG
	AddComponent<InspectorExtension>(new InspectorContentParticleOfLight);
#endif

	name = "ParticleOfLight";
	transform.position.z = -15.0f;
	particle = AddComponent<ParticleSystem>(100);
	particle->SetLayer(Layer::MASK);
	particle->SetBehavior(&behavior);
	particle->loop = true;
	particle->pTexture = Texture::Get("particle_light");
	particle->emission_rate = 10.0f;

	if (SeasonManager::GetSeason() == SeasonType::Winter)
		SetActive(false);
}

void ParticleOfLight::OnDraw()
{
	auto pDevice = Direct3D::GetDevice();
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void ParticleOfLight::AfterDraw()
{
	auto pDevice = Direct3D::GetDevice();
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

void ParticleOfLight::SetSummer(void)
{
	SetActive(true);
}

void ParticleOfLight::SetWinter(void)
{
	SetActive(false);
}


LightParticleBehavior::LightParticleBehavior(MapManager* map)
{
	camera = &RenderSpace::Get("default")->GetCamera(0)->transform;
	this->map = map;
}

void LightParticleBehavior::Init(ParticleElement & element)
{
	element.timer.Reset(Randomf(3, 5));
	element.transform.scale = Vector3::one*Randomf(0.6f, 1.4f);
	element.color = Color(255, 255, 255, (int)((element.timer.interval - element.timer.Elapsed()) * 20));
	do
		element.init_pos.x = camera->position.x + Randomf(-camera_range, camera_range);
	while (element.init_pos.x < 0);
	element.transform.position.y = map->GetGroundPosition(element.init_pos.x);
	element.transform.position.z = Randomf(-4.9f, 5.f);
	element.random_seed = Randomf(0.0f, 10.0f);
}

void LightParticleBehavior::Update(ParticleElement & element)
{
	element.timer++;
	element.color = Color(255, 255, 255, (int)((element.timer.interval - element.timer.Elapsed()) * 20));
	element.transform.position.y += rise_speed*Time::DeltaTime();
	element.transform.position.x = element.init_pos.x + noise_scale*PerlinNoise(element.random_seed + element.transform.position.y*noise_frequency, noise_octavers);

	if (element.timer.TimeUp())
		element.active = false;
}
