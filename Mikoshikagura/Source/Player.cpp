#include "Player.h"


//=============================================================================
// Player
//=============================================================================
#pragma region Player
Player::Player(void)
{
	this->type = ObjectType::Player;
	this->speed = PlayerSpeed;
	this->anime = AnimeSet::Idle;
	this->is_grounded = false;

	// モデル初期化
	this->model = AddComponent<SkinnedModel>("player");
	this->model->SetAnime((int)this->anime);

	// コライダー初期化
	this->collider = AddComponent<BoxCollider2D>();
	this->collider->size = Vector2(4.0f, 16.0f);
	this->collider->offset.y += 0.5f*this->collider->size.y;

	this->rigidbody = AddComponent<Rigidbody>();
	this->rigidbody->useGravity = false;

	// ステート初期化
	this->state.resize((int)StateName::Max);
	this->state[(int)StateName::Idle	].reset(new StateIdle(this));
	this->state[(int)StateName::Move	].reset(new StateMove(this));
	this->state[(int)StateName::Air		].reset(new StateAir(this));
	this->state[(int)StateName::Action	].reset(new StateAction(this));
	this->current_state = this->state[(int)StateName::Idle].get();
}

void Player::Update(void)
{
	MoveControl();
	ActionControl();

	this->current_state->Update();
	this->is_grounded = false;
	this->last_position = this->transform.position;
}

void Player::Uninit(void)
{
}

void Player::OnCollision(Object * other)
{
	if (other->type == ObjectType::Object)
	{
		auto otherCollider = other->GetComponent<BoxCollider2D>();

		// 着地判定
		if (this->last_position.y + this->collider->offset.y - 0.5f*this->collider->size.y >=
			other->transform.position.y + otherCollider->offset.y + 0.5f*otherCollider->size.y)
		{
			this->is_grounded = true;
		}

		// 衝突応答の処理
		auto x = 0.0f;
		auto y = 0.0f;

		// めり込みの量を計算する
		if (this->rigidbody->velocity.x > 0.0f)
		{
			x = (this->rigidbody->position.x + this->collider->offset.x + 0.5f*this->collider->size.x) -
				(other->transform.position.x + otherCollider->offset.x - 0.5f*otherCollider->size.x);
		}
		else if(this->rigidbody->velocity.x < 0.0f)
		{
			x = (this->rigidbody->position.x + this->collider->offset.x - 0.5f*this->collider->size.x) -
				(other->transform.position.x + otherCollider->offset.x + 0.5f*otherCollider->size.x);
		}
		else
		{
			auto x1 = (this->rigidbody->position.x + this->collider->offset.x + 0.5f*this->collider->size.x) -
				(other->transform.position.x + otherCollider->offset.x - 0.5f*otherCollider->size.x);
			auto x2 = (this->rigidbody->position.x + this->collider->offset.x - 0.5f*this->collider->size.x) -
				(other->transform.position.x + otherCollider->offset.x + 0.5f*otherCollider->size.x);
			x = fminf(fabsf(x1), fabsf(x2));
		}

		if (this->rigidbody->velocity.y > 0.0f)
		{
			y = (this->rigidbody->position.y + this->collider->offset.y + 0.5f*this->collider->size.y) -
				(other->transform.position.y + otherCollider->offset.y - 0.5f*otherCollider->size.y);
		}
		else if(this->rigidbody->velocity.y < 0.0f)
		{
			y = (this->rigidbody->position.y + this->collider->offset.y - 0.5f*this->collider->size.y) -
				(other->transform.position.y + otherCollider->offset.y + 0.5f*otherCollider->size.y);
		}
		else
		{
			auto y1 = (this->rigidbody->position.y + this->collider->offset.y + 0.5f*this->collider->size.y) -
				(other->transform.position.y + otherCollider->offset.y - 0.5f*otherCollider->size.y);
			auto y2 = (this->rigidbody->position.y + this->collider->offset.y - 0.5f*this->collider->size.y) -
				(other->transform.position.y + otherCollider->offset.y + 0.5f*otherCollider->size.y);
			y = fminf(fabsf(y1), fabsf(y2));
		}

		// めり込んだ量だけ押し返す
		if (fabsf(x) <= fabsf(y))
		{
			this->rigidbody->position.x -= x;
			this->rigidbody->velocity.x = 0.0f;
			this->transform.position.x = this->rigidbody->position.x;
		}
		else
		{
			this->rigidbody->position.y -= y;
			this->rigidbody->velocity.y = 0.0f;
			this->transform.position.y = this->rigidbody->position.y;
		}

	}
}

void Player::SetPosition(Vector3 pos)
{
	this->rigidbody->position = pos;
}

void Player::SetAnime(AnimeSet anime, bool loop)
{
	int index = (int)anime;

	this->model->SetAnime(index);
	this->anime = anime;

	if (loop)
		this->anime_timer.Reset(-1);
	else
		this->anime_timer.Reset(this->model->GetAnimePeriod(index));

}

void Player::MoveControl(void)
{
	this->control = Vector3::zero;

	// キーボード入力
	if (GetKeyboardPress(DIK_W))
		control += Vector3(0.0f, 0.0f, 1.0f);
	if (GetKeyboardPress(DIK_S))
		control += Vector3(0.0f, 0.0f, -1.0f);
	if (GetKeyboardPress(DIK_A))
		control += Vector3(-1.0f, 0.0f, 0.0f);
	if (GetKeyboardPress(DIK_D))
		control += Vector3(1.0f, 0.0f, 0.0f);

	if (GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D))
		control = control.normalized();

	// パッド入力
	control += Vector3(GetPadLX(), 0, -GetPadLY());

	// ムーブイベント
	if (this->control.sqrLength() > 0.0f)
	{
		this->current_state->SetState(StateName::Move);
	}
	else
	{
		this->rigidbody->velocity.x = 0.0f;
	}
}

void Player::ActionControl(void)
{
	if (GetKeyboardTrigger(KeyAction) && action)
	{
		action();
	}
}

bool Player::JumpControl(void)
{
	if (GetKeyboardTrigger(KeyJump))
	{
		this->rigidbody->position.y += 1.0f;
		this->rigidbody->velocity.y = PlayerJumpSpeed;
		this->current_state->SetState(StateName::Air);
		return true;
	}

	return false;
}

void Player::Move(void)
{
	// 移動処理
	auto camera = Renderer::GetInstance()->getCamera();
	Vector3 offset = this->transform.position - camera->transform.position;
	float phi = atan2f(offset.z, offset.x) - 0.5f*PI;

	Vector3 move;
	move.x = control.x*cosf(phi) - control.z*sinf(phi);
	move.z = control.x*sinf(phi) + control.z*cosf(phi);

	// 移動量を反応
	this->rigidbody->velocity.x = move.x*PlayerSpeed;

	// 向きの設定
	this->transform.setFront(move);
}

#pragma endregion


//=============================================================================
// State
//=============================================================================

void Player::State::SetState(StateName state)
{
	this->player->current_state->OnExit();
	this->player->current_state = player->state[(int)state].get();
	this->player->current_state->OnEnter();
}

#pragma region StateIdle
//=============================================================================
// StateIdle
//=============================================================================

void Player::StateIdle::OnEnter(void)
{
	this->player->SetAnime(AnimeSet::Idle);
}

void Player::StateIdle::Update(void)
{
	if (this->player->is_grounded)
	{
		this->player->JumpControl();
	}
	else
	{
		SetState(StateName::Air);
	}
}

void Player::StateIdle::SetState(StateName state)
{
	switch (state)
	{
	case StateName::Move:
	case StateName::Air:
	case StateName::Action:
		State::SetState(state);
	}
}

#pragma endregion

#pragma region StateMove
//=============================================================================
// StateMove
//=============================================================================

void Player::StateMove::OnEnter(void)
{
	this->player->SetAnime(AnimeSet::Running);
}

void Player::StateMove::Update(void)
{
	if (this->player->is_grounded)
	{
		if (!this->player->JumpControl())
		{
			float control_len = this->player->control.length();
			if (control_len > 0.0f)
			{
				this->player->Move();
				this->player->model->SetAnimeSpeedScale(control_len);
			}
			else
			{
				this->SetState(StateName::Idle);
			}
		}
	}
	else
	{
		SetState(StateName::Air);
	}
}

void Player::StateMove::OnExit(void)
{
	this->player->model->SetAnimeSpeedScale(1.0f);
}

void Player::StateMove::SetState(StateName state)
{
	switch (state)
	{
	case StateName::Idle:
	case StateName::Air:
	case StateName::Action:
		State::SetState(state);
	}
}

#pragma endregion

#pragma region StateAction
//=============================================================================
// StateAttack
//=============================================================================
void Player::StateAction::OnEnter(void)
{
}

void Player::StateAction::Update(void)
{
}

void Player::StateAction::SetState(StateName state)
{
	switch (state)
	{
	case StateName::Idle:
		State::SetState(state);
		break;
	}
}
#pragma endregion

#pragma region StateAir
//=============================================================================
// StateAir
//=============================================================================

void Player::StateAir::OnEnter(void)
{
	//this->player->transform.position.y += 1.0f;
	this->player->is_grounded = false;
	this->player->rigidbody->useGravity = true;

	//TODO: ジャンプ／落下のアニメーションにする
	this->player->SetAnime(AnimeSet::Idle);

	//this->player->rigidbody->SetActive(true);
}

void Player::StateAir::Update(void)
{
	auto control_len = this->player->control.length();
	if (control_len > 0.0f)
	{
		this->player->Move();
	}

	if (this->player->is_grounded)
	{
		if (control_len > 0.0f)
			SetState(StateName::Move);
		else
			SetState(StateName::Idle);
	}

}

void Player::StateAir::OnExit(void)
{
	this->player->rigidbody->velocity.y = 0.0f;
	this->player->rigidbody->acceleration.y = 0.0f;
	this->player->rigidbody->useGravity = false;
	//this->player->rigidbody->SetActive(false);
}

void Player::StateAir::SetState(StateName state)
{
	switch (state)
	{
	case StateName::Idle:
	case StateName::Move:
		if (this->player->is_grounded)
			State::SetState(state);
	}
}

#pragma endregion