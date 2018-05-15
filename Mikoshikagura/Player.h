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
	// 定数定義

	static constexpr int MaxHp = 20;
	static constexpr int MaxAtk = 3;
	static constexpr float Radius = 2.3f;

	// ステート定義
	enum class StateName
	{
		Idle,
		Move,
		Attack,
		Damage,
		Max
	};

	// アニメーション定義
	enum class AnimeSet
	{
		Running,
		Injure,
		ShootBulletShort,
		AttackLong,
		AttackArea,
		Idle
	} anime;

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
		Player* player;

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

	// 攻撃状態
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

	// 負傷状態
	class StateDamage : public State
	{
	public:
		StateDamage(Player* player) : State(player) {}
		void OnEnter(void) override;
		void Update(void) override;
		void SetState(StateName state) override;
	};

	// 状態インスタンスリスト
	std::vector<smart_ptr<State>> state;

	// メンバー変数定義

	int atk;
	Event event_move;
	Event event_get_element;

	// メンバー関数定義

	Player(void);
	void Update(void) override;
	void Uninit(void) override;
	void OnCollision(Object* other) override;
	// プレイヤーのATK値を1単位上げる、MAXになるとそれ以上増えない。
	void AtkUp(void);
	int GetElementNum(void);


private:
	// メンバー変数定義

	SkinnedModel* model;
	SphereCollider* collider;
	Vector3 control;
	FrameTimer anime_timer;
	FrameTimer bullet_timer;
	float speed;
	int element_num;
	std::function<void(void)> init_attack;
	std::function<void(void)> update_attack;


	// メンバー関数定義

	void SetAnime(AnimeSet anime, bool loop = true);
	void MoveControl(void);
	void Move(void);
	void AttackControl(void);
	void ShootBulletShort(void);
	void ShootBulletLong(void);
	void ShootBulletArea(void);
};