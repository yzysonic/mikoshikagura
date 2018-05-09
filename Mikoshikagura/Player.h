#pragma once
#include "Core/Core.h"

#define KeyAtkShort	DIK_J
#define KeyAtkLong	DIK_K
#define KeyAtkArea	DIK_L
#define BtnAtkShort	BUTTON_SQ
#define BtnAtkLong	BUTTON_TR
#define BtnAtkArea	BUTTON_CI

#define PlayerSpeed (20.0f)

class Player : public Object
{
public:
	// �萔��`

	static constexpr int MaxHp = 20;
	static constexpr int MaxAtk = 3;
	static constexpr float Radius = 2.3f;

	// �X�e�[�g��`
	enum class StateName
	{
		Idle,
		Move,
		Attack,
		Damage,
		Max
	};

	// �A�j���[�V������`
	enum class AnimeSet
	{
		Running,
		Injure,
		ShootBulletShort,
		AttackLong,
		AttackArea,
		Idle
	} anime;

	// ��ԃN���X
	class State
	{
	public:
		State(Player* player) : player(player) {}
		virtual void OnEnter(void) {}
		virtual void Update(void) {}
		virtual void OnExit(void) {}
		virtual void SetState(StateName state);

	protected:
		Player* player;

	} *current_state;

	// �ҋ@���
	class StateIdle : public State
	{
	public:
		StateIdle(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void SetState(StateName state) override;
	};

	// �ړ����
	class StateMove : public State
	{
	public:
		StateMove(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void OnExit(void) override;
		void SetState(StateName state) override;
	};

	// �U�����
	class StateAttack : public State
	{
	public:
		StateAttack(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void SetState(StateName state) override;
	private:
		FrameTimer timer;
	};

	// �������
	class StateDamage : public State
	{
	public:
		StateDamage(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void SetState(StateName state) override;
	};

	// ��ԃC���X�^���X���X�g
	std::vector<smart_ptr<State>> state;

	// �����o�[�ϐ���`

	int atk;
	Event event_move;
	Event event_get_element;

	// �����o�[�֐���`

	Player(void);
	void Update(void) override;
	void Uninit(void) override;
	void OnCollision(Object* other) override;
	// �v���C���[��ATK�l��1�P�ʏグ��AMAX�ɂȂ�Ƃ���ȏ㑝���Ȃ��B
	void AtkUp(void);
	int GetElementNum(void);


private:
	// �����o�[�ϐ���`

	SkinnedModel* model;
	SphereCollider* collider;
	Vector3 control;
	FrameTimer anime_timer;
	FrameTimer bullet_timer;
	float speed;
	int element_num;
	std::function<void(void)> init_attack;
	std::function<void(void)> update_attack;


	// �����o�[�֐���`

	void SetAnime(AnimeSet anime, bool loop = true);
	void MoveControl(void);
	void Move(void);
	void AttackControl(void);
	void ShootBulletShort(void);
	void ShootBulletLong(void);
	void ShootBulletArea(void);
};