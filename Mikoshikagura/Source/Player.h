#pragma once
#include "Core/Core.h"
#include "ISeason.h"
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

class Player : public Object, public ISeason
{
	friend class InspectorContentPlayer;
public:
	// 定数定義

	static constexpr int MaxHp = 20;
	static constexpr int MaxAtk = 3;
	static constexpr float Radius = 2.3f;

	// ステート定義
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
	// 状態クラス
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

	// 待機状態
	class StateIdle : public State
	{
	public:
		StateIdle(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void SetState(StateName state) override;
		inline const char* ToString(void) override { return "Idle"; }
	};

	// 移動状態
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

	// 空中状態
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

	// 行動状態
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

	// 吹笛状態
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

	// 状態インスタンスリスト
	std::vector<smart_ptr<State>> state;
	Event season_change;

	// メンバー関数定義

	Player(void);
	void Update(void) override;
	void Uninit(void) override;
	void OnCollisionEnter(Object*  other) override;
	void OnCollisionStay(Object* other) override;
	void OnCollisionExit(Object* other) override;
	void SetSummer(void) override;
	void SetWinter(void) override;
	void SetPosition(Vector3 pos);


private:
	// メンバー変数定義

	SkinnedModel* model;
	std::string current_animation;
	BoxCollider2D* collider;
	std::set<Collider*> ground_colliders;
	Rigidbody* rigidbody;
	std::vector<Sound*> foot_sounds;
	std::vector<Sound*> jump_sounds;
	Sound * skill_sound;
	Vector3 control;
	Vector3 last_position;
	FrameTimer anime_timer;
	FrameTimer jump_timer;
	float speed;
	bool is_grounded;
	bool is_holding_item;
	Event action_enter;
	Event action_update;
	Event action_exit;
	Action PlayFootSound;

	// メンバー関数定義

	void SetAnimation(std::string name, bool loop = true);
	void MoveControl(void);
	void ActionControl(void);
	void SeasonChangeControl(void);
	bool JumpControl(void);
	void Move(void);
};