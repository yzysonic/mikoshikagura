#pragma once

#include "Core.h"
#include <functional>
#include <unordered_map>

struct VtxParticleGeometry
{
	Vector3 pos;
	Vector2 uv;
};

struct VtxParticleInstance
{
	D3DXMATRIX	mtx_world;
	Color		color;
};

struct ParticleElement
{
	Transform transform;
	Color color;
	Vector3 init_pos;
	Vector3 dir;
	FrameTimer timer;
	float random_seed;
	bool active = false;
};

class IParticleBehavior
{
public:
	virtual void MakeElement(ParticleElement** elements, UINT* pitch, UINT number);
	virtual void Init(ParticleElement & element) = 0;
	virtual void Update(ParticleElement & element) = 0;
};

class ParticleDefaultBehavior : public IParticleBehavior
{
public:
	float lifetime;
	float damping;
	float start_speed;
	float start_size;
	float start_opacity;
	Color start_color;

	float end_size;
	float end_opacity;
	Color end_color;

	ParticleDefaultBehavior(void);
	void Init(ParticleElement & element) override;
	void Update(ParticleElement & element) override;
	void SetTimingFunc(std::string attr, std::function<float(float, float, float)> func);

private:
	std::unordered_map<std::string, std::function<float(float, float, float)>> timing_func;
};

class ParticleSystem : public Drawable, public ILostAndReset
{
public:
	float emission_rate;
	bool loop;
	Texture* pTexture;

	ParticleSystem(UINT particle_max = 100, IParticleBehavior* behavior = nullptr);
	~ParticleSystem(void);

	void Draw(void) override;
	HRESULT OnLostDevice(void) override;
	HRESULT OnResetDevice(void) override;

	void SetMaxNum(UINT value);
	UINT GetMaxNum(void);
	void SetBehavior(IParticleBehavior* behavior = nullptr);
	void SetDuration(float value);
	UINT GetParticleNum(void);
	template<class T>
	T* GetBehavior(void);

private:
	IDirect3DVertexDeclaration9 *decl;
	LPDIRECT3DINDEXBUFFER9 pIndexBuff;
	LPDIRECT3DVERTEXBUFFER9 pGeometryBuff;
	LPDIRECT3DVERTEXBUFFER9 pInstanceBuff;

	ParticleElement* elements;
	IParticleBehavior* behavior;
	ParticleDefaultBehavior* default_behavior;
	FrameTimer timer_duration;
	FrameTimer timer_emission;
	UINT particle_max;
	UINT particle_num;
	UINT pitch;
	float duration;

	void Update(void);
	void InitDraw(void);
	void UninitDraw(void);
};

template<class T>
inline T * ParticleSystem::GetBehavior(void)
{
	static_assert(std::is_base_of<IParticleBehavior, T>::value, "Not a subclass of IParticleBehavior.");
	return dynamic_cast<T*>(this->behavior);
}
