#pragma once
#include "Core/Core.h"
#include <set>

#define KeyJump			DIK_SPACE
#define KeyAction		DIK_RETURN
#define KeyAction2		DIK_E
#define KeySeasonChange	DIK_TAB

#define ButtonJump			BUTTON_CR
#define ButtonAction		BUTTON_CI
#define ButtonSeasonChange	BUTTON_TR

#define PlayerSpeed (40.0f)
#define PlayerJumpSpeed (45.0f)

class Player : public Object
{
	friend class InspectorContentPlayer;
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
		SeasonChange,
		Max
	};

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

	} *current_state, *action_state;

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
		StateMove(Player* player) : State(player), running(false) {}
		void OnEnter(void) override;
		void Update(void) override;
		void OnExit(void) override;
		void SetState(StateName state) override;
		inline const char* ToString(void) override { return "Move"; }

	private:
		bool running;
		void SetRunning(void);
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

	// �s�����
	class StateAction : public State
	{
	public:
		StateAction(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void OnExit(void) override;
		void SetState(StateName state) override;
		inline const char* ToString(void) override { return "Action"; }
	private:
		FrameTimer timer;
	};

	// ���J���
	class StateSeasonChange : public State
	{
	public:
		StateSeasonChange(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void SetState(StateName state) override;
		inline const char* ToString(void) override { return "SeasonChange"; }
	private:
		bool change;
	};

#pragma endregion

#pragma region Foot
	class Foot : public Object
	{
		friend class InspectorContentPlayer;
	public:
		static Player* player;
		BoxCollider2D* collider;

		Foot(const char* name);
		void Update(void) override;
		void OnCollisionEnter(Object* other) override;
		void OnCollisionExit(Object* other) override;

	private:
		BoneFrame * frame;
		Object* last_ground;
		float last_y;
	};
#pragma endregion


	// ��ԃC���X�^���X���X�g
	std::vector<smart_ptr<State>> state;
	Event season_change;

	// �����o�[�֐���`

	Player(void);
	void Update(void) override;
	void Uninit(void) override;
	void OnCollisionEnter(Object*  other) override;
	void OnCollisionStay(Object* other) override;
	void OnCollisionExit(Object* other) override;
	void SetPosition(Vector3 pos);


private:
	// �����o�[�ϐ���`

	SkinnedModel* model;
	std::string current_animation;
	BoxCollider2D* collider;
	std::set<Collider*> ground_colliders;
	Rigidbody* rigidbody;
	Foot* foot[2];
	SoundPlayer* sound_player;
	std::vector<Sound*> foot_sounds;
	Vector3 control;
	Vector3 last_position;
	FrameTimer anime_timer;
	float speed;
	bool is_grounded;
	bool is_holding_item;
	Event action_enter;
	Event action_update;
	Event action_exit;

	// �����o�[�֐���`

	void SetAnimation(std::string name, bool loop = true);
	void MoveControl(void);
	void ActionControl(void);
	void SeasonChangeControl(void);
	bool JumpControl(void);
	void Move(void);
};