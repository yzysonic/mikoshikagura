#pragma once
#include "Core/Core.h"

#define KeyJump		DIK_SPACE
#define KeyAction	DIK_RETURN
#define KeyWhistle	DIK_TAB

#define PlayerSpeed (20.0f)
#define PlayerJumpSpeed (45.0f)

class Player : public Object
{
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
		Max
	};

	// アニメーション定義
	enum class AnimeSet
	{
		Running,
		Idle
	} anime;

	
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

	protected:
		Player * player;

	} *current_state;

	// 待機状態
	class StateIdle : public State
	{
	public:
		StateIdle(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void SetState(StateName state) override;
	};

	// 移動状態
	class StateMove : public State
	{
	public:
		StateMove(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void OnExit(void) override;
		void SetState(StateName state) override;
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
	};

	// 攻撃状態
	class StateAction : public State
	{
	public:
		StateAction(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void SetState(StateName state) override;
	private:
		FrameTimer timer;
	};

#pragma endregion


	// 状態インスタンスリスト
	std::vector<smart_ptr<State>> state;

	// メンバー関数定義

	Player(void);
	void Update(void) override;
	void Uninit(void) override;
	void OnCollision(Object* other) override;
	void SetPosition(Vector3 pos);


private:
	// メンバー変数定義

	SkinnedModel* model;
	BoxCollider2D* collider;
	Rigidbody* rigidbody;
	Vector3 control;
	Vector3 last_position;
	FrameTimer anime_timer;
	float speed;
	bool is_grounded;
	std::function<void(void)> action;


	// メンバー関数定義

	void SetAnime(AnimeSet anime, bool loop = true);
	void MoveControl(void);
	void ActionControl(void);
	bool JumpControl(void);
	void Move(void);
};