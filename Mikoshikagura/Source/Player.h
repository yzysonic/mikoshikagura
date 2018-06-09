#pragma once
#include "Core/Core.h"
#include <set>

#define KeyJump		DIK_SPACE
#define KeyAction	DIK_RETURN
#define KeyWhistle	DIK_TAB

#define PlayerSpeed (20.0f)
#define PlayerJumpSpeed (45.0f)

class Player : public Object
{
#ifdef _DEBUG
	friend class InspectorContentPlayer;
#endif
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
		Air,
		Action,
		Max
	};

	// �A�j���[�V������`
	enum class AnimeSet
	{
		Running,
		Idle
	} anime;

	
#pragma region State
	// ��ԃN���X
	class State
	{
	public:
		State(Player* player) : player(player) {}
		virtual void OnEnter(void) {}
		virtual void Update(void) {}
		virtual void OnExit(void) {}
		virtual void SetState(StateName state);
		virtual const char* ToString(void) = 0;

	protected:
		Player * player;

	} *current_state;

	// �ҋ@���
	class StateIdle : public State
	{
	public:
		StateIdle(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void SetState(StateName state) override;
		inline const char* ToString(void) override { return "Idle"; }
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
		inline const char* ToString(void) override { return "Move"; }
	};

	// �󒆏��
	class StateAir : public State
	{
	public:
		StateAir(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void OnExit(void) override;
		void SetState(StateName state) override;
		inline const char* ToString(void) override { return "Air"; }
	};

	// �U�����
	class StateAction : public State
	{
	public:
		StateAction(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void SetState(StateName state) override;
		inline const char* ToString(void) override { return "Action"; }
	private:
		FrameTimer timer;
	};

#pragma endregion


	// ��ԃC���X�^���X���X�g
	std::vector<smart_ptr<State>> state;

	// �����o�[�֐���`

	Player(void);
	void Update(void) override;
	void Uninit(void) override;
	void OnCollisionStay(Object* other) override;
	void OnCollisionExit(Object* other) override;
	void SetPosition(Vector3 pos);


private:
	// �����o�[�ϐ���`

	SkinnedModel* model;
	BoxCollider2D* collider;
	std::set<Collider*> ground_colliders;
	Rigidbody* rigidbody;
	Vector3 control;
	Vector3 last_position;
	FrameTimer anime_timer;
	float speed;
	bool is_grounded;
	std::function<void(void)> action;


	// �����o�[�֐���`

	void SetAnime(AnimeSet anime, bool loop = true);
	void MoveControl(void);
	void ActionControl(void);
	bool JumpControl(void);
	void Move(void);
};