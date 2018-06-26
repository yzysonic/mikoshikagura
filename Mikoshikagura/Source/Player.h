#pragma once
#include "Core/Core.h"
#include <set>

#define KeyJump		DIK_SPACE
#define KeyAction	DIK_RETURN
#define KeyWhistle	DIK_TAB

#define ButtonJump		BUTTON_CR
#define ButtonAction	BUTTON_CI
#define ButtonWhistle	BUTTON_TR

#define PlayerSpeed (40.0f)
#define PlayerJumpSpeed (45.0f)

class Player : public Object
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
		Whistle,
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
		virtual const char* ToString(void) = 0;

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
		inline const char* ToString(void) override { return "Idle"; }
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
		inline const char* ToString(void) override { return "Move"; }
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
		void SetState(StateName state) override;
		inline const char* ToString(void) override { return "Action"; }
	private:
		FrameTimer timer;
	};

	// 吹笛状態
	class StateWhistle : public State
	{
	public:
		StateWhistle(Player* player) : State(player) {}
		void OnEnter(void) override;
		void SetState(StateName state) override;
		inline const char* ToString(void) override { return "Whistle"; }
	private:
		FrameTimer timer;
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


	// 状態インスタンスリスト
	std::vector<smart_ptr<State>> state;
	Event whistle;

	// メンバー関数定義

	Player(void);
	void Update(void) override;
	void Uninit(void) override;
	void OnCollisionEnter(Object*  other) override;
	void OnCollisionStay(Object* other) override;
	void OnCollisionExit(Object* other) override;
	void SetPosition(Vector3 pos);


private:
	// メンバー変数定義

	SkinnedModel* model;
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
	Event action;

	// メンバー関数定義

	void SetAnime(AnimeSet anime, bool loop = true);
	void MoveControl(void);
	void ActionControl(void);
	void WhistleControl(void);
	bool JumpControl(void);
	void Move(void);
};