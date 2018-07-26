#include "Player.h"
#include "Core/Physics.h"
#include "Holdable.h"
#include "SeasonManager.h"

#ifdef _DEBUG
#include "InspectorContentPlayer.h"
#endif

//=============================================================================
// Player
//=============================================================================
#pragma region Player
Player::Player(void)
{
	this->type = ObjectType::Player;
	this->name = "Player";
	this->speed = PlayerSpeed;
	this->current_animation = "Idle";
	this->is_grounded = false;
	this->is_holding_item = false;
	this->jump_timer.Reset(0.0f);

	// モデル初期化
	this->model = AddComponent<SkinnedModel>("player");
	this->model->SetAnimation(this->current_animation);

	// コライダー初期化
	this->collider = AddComponent<BoxCollider2D>();
	this->collider->size = Vector2(4.0f, 16.0f);
	this->collider->offset.y += 0.5f*this->collider->size.y;

	// Rigidbody初期化
	this->rigidbody = AddComponent<Rigidbody>();
	this->rigidbody->useGravity = true;

	// サウンド初期化
	this->jump_sounds = Sound::GetSerial("jump");
	this->skill_sound = Sound::Get("player_skill");

	// 足音初期化
	this->foot_sounds = Sound::GetSerial( (SeasonManager::GetSeason() == SeasonType::Summer) ? "foot_mud" : "foot_snow");

	this->PlayFootSound = [this]
	{
		foot_sounds[Random(0, foot_sounds.size() - 1)]->Play();
	};

	// 足音のアニメーションイベント登録
	auto animSet = (ID3DXKeyframedAnimationSet*)this->model->GetAnimation("Walk");

	D3DXKEY_CALLBACK keys[2];
	keys[0].Time = 12.0f;
	keys[1].Time = 27.0f;

	keys[0].pCallbackData = keys[1].pCallbackData = &PlayFootSound;

	ID3DXBuffer* compressedInfo = 0;
	animSet->Compress(D3DXCOMPRESS_DEFAULT, 0.5f, 0, &compressedInfo);

	ID3DXCompressedAnimationSet* compressedAnimSet = 0;
	D3DXCreateCompressedAnimationSet(animSet->GetName(),
		animSet->GetSourceTicksPerSecond(),
		animSet->GetPlaybackType(), compressedInfo,
		2, keys, &compressedAnimSet);

	this->model->animator->UnregisterAnimationSet(animSet);
	this->model->animator->RegisterAnimationSet(compressedAnimSet);

	compressedInfo->Release();
	animSet->Release();
	compressedAnimSet->Release();

	animSet = (ID3DXKeyframedAnimationSet*)this->model->GetAnimation("Run");

	keys[0].Time = 9.0f;
	keys[1].Time = 20.0f;

	animSet->Compress(D3DXCOMPRESS_DEFAULT, 0.5f, 0, &compressedInfo);

	D3DXCreateCompressedAnimationSet(animSet->GetName(),
		animSet->GetSourceTicksPerSecond(),
		animSet->GetPlaybackType(), compressedInfo,
		2, keys, &compressedAnimSet);

	this->model->animator->UnregisterAnimationSet(animSet);
	this->model->animator->RegisterAnimationSet(compressedAnimSet);

	compressedInfo->Release();
	animSet->Release();
	compressedAnimSet->Release();


	// ステート初期化
	this->state.resize((int)StateName::Max);
	this->state[(int)StateName::Idle	].reset(new StateIdle(this));
	this->state[(int)StateName::Move	].reset(new StateMove(this));
	this->state[(int)StateName::Air		].reset(new StateAir(this));
	this->state[(int)StateName::Action	].reset(new StateAction(this));
	this->action_state = this->state[3].get();
	this->state[(int)StateName::SeasonChange	].reset(new StateSeasonChange(this));
	this->current_state = this->state[(int)StateName::Idle].get();

#ifdef _DEBUG
	this->AddComponent<InspectorExtension>(new InspectorContentPlayer);
#endif

}

void Player::Update(void)
{
	jump_timer++;

	MoveControl();
	ActionControl();
	SeasonChangeControl();

	this->current_state->Update();
	this->last_position = this->transform.position;
}

void Player::Uninit(void)
{

}

void Player::OnCollisionEnter(Object * other)
{
	if (other->type == ObjectType::Item && !this->action_enter)
	{
		auto item = other->GetComponent<Holdable>();

		// ものを拾う
		this->action_enter = [item, this]
		{
			SetAnimation("Pick", false);
			this->rigidbody->velocity.x = 0.0f;

			// ものを放つ
			this->action_enter = [item, this]
			{
				item->SetOwner(nullptr);
				this->is_holding_item = false;
				this->action_enter = nullptr;
				this->action_update = nullptr;
				this->action_exit = nullptr;

				this->current_state->SetState(StateName::Idle);
			};

		};

		this->action_update = [item, this]
		{
			this->anime_timer++;
			if (!this->is_holding_item && this->anime_timer.Elapsed() > 0.833f)
			{
				this->is_holding_item = true;
				item->offset_y = this->collider->size.y + 0.5f*item->object->GetComponent<BoxCollider2D>()->size.y + 2.0f;
				item->SetOwner(&this->transform);
			}
			if (this->anime_timer.TimeUp())
			{
				this->current_state->SetState(StateName::Idle);
			}
		};
	}
}

void Player::OnCollisionStay(Object * other)
{
	if (other->type == ObjectType::Field)
	{
		auto otherCollider = other->GetComponent<BoxCollider2D>();
		auto otherColliderPos = other->transform.position.toVector2() + otherCollider->offset;

		// 着地判定
		if (this->last_position.y + this->collider->offset.y - 0.5f*this->collider->size.y >=
			otherColliderPos.y + 0.5f*otherCollider->size.y)
		{
			this->ground_colliders.insert(otherCollider);
			this->is_grounded = true;
		}

		// 衝突応答の処理
		auto diff = Vector2(0.0f, 0.0f);

		// めり込みの量を計算する
		if(this->rigidbody->position.x < otherColliderPos.x)
		{
			diff.x = (this->rigidbody->position.x + this->collider->offset.x + 0.5f*this->collider->size.x) -
				(otherColliderPos.x - 0.5f*otherCollider->size.x);
			diff.x += 0.0001f;
		}
		else
		{
			diff.x = (this->rigidbody->position.x + this->collider->offset.x - 0.5f*this->collider->size.x) -
				(otherColliderPos.x + 0.5f*otherCollider->size.x);
			diff.x -= 0.0001f;
		}

		if (this->rigidbody->position.y < otherColliderPos.y)
		{
			diff.y = (this->rigidbody->position.y + this->collider->offset.y + 0.5f*this->collider->size.y) -
				(otherColliderPos.y - 0.5f*otherCollider->size.y);
		}
		else
		{
			diff.y = (this->rigidbody->position.y + this->collider->offset.y - 0.5f*this->collider->size.y) -
				(otherColliderPos.y + 0.5f*otherCollider->size.y);
		}

		// めり込んだ量だけ押し返す
		if (fabsf(diff.x) <= fabsf(diff.y))
		{
			this->rigidbody->position.x -= diff.x;
			this->rigidbody->velocity.x = 0.0f;
			this->transform.position.x = this->rigidbody->position.x;
		}
		else
		{
			this->rigidbody->position.y -= diff.y;
			this->rigidbody->velocity.y = 0.0f;
			this->transform.position.y = this->rigidbody->position.y;
		}

	}
}

void Player::OnCollisionExit(Object * other)
{
	switch (other->type)
	{
	case ObjectType::Field:
	{
		auto collider = other->GetComponent<BoxCollider2D>();
		if (this->ground_colliders.find(collider) != this->ground_colliders.end())
		{
			this->ground_colliders.erase(collider);

			if (this->ground_colliders.empty())
				this->is_grounded = false;
		}
	}
		break;

	case ObjectType::Item:
		if (!this->is_holding_item)
		{
			this->action_enter = nullptr;
		}
	}
	
}

void Player::SetSummer(void)
{
	this->foot_sounds = Sound::GetSerial("foot_mud");
}

void Player::SetWinter(void)
{
	this->foot_sounds = Sound::GetSerial("foot_snow");
}

void Player::SetPosition(Vector3 pos)
{
	this->transform.position = pos;
	this->rigidbody->position = pos;
}

void Player::SetAnimation(std::string name, bool loop)
{
	this->model->SetAnimation(name);
	this->current_animation = name;

	if (loop)
		this->anime_timer.Reset(-1);
	else
		this->anime_timer.Reset(this->model->GetAnimationPeriod(name));

}

void Player::MoveControl(void)
{
	this->control = Vector3::zero;

	// キーボード入力
	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFT) || IsButtonPressed(BUTTON_LEFT))
		control += Vector3(-1.0f, 0.0f, 0.0f);
	if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(BUTTON_RIGHT))
		control += Vector3(1.0f, 0.0f, 0.0f);

	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT))
		control = control.normalized() * (GetKeyboardPress(DIK_LSHIFT)? 1.0f : 0.5f);

	// パッド入力
	control += Vector3(GetPadLX(), 0.0f, 0.0f);

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
	if ((GetKeyboardTrigger(KeyAction) || GetKeyboardTrigger(KeyAction2) || IsButtonTriggered(ButtonAction)) && this->action_enter)
	{
		this->current_state->SetState(StateName::Action);
	}
}

void Player::SeasonChangeControl(void)
{
	if ((GetKeyboardTrigger(KeySeasonChange) || IsButtonTriggered(ButtonSeasonChange)) && !this->is_holding_item)
	{
		this->current_state->SetState(StateName::SeasonChange);
	}
}

bool Player::JumpControl(void)
{
	if (GetKeyboardTrigger(KeyJump) || IsButtonTriggered(ButtonJump))
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
	Vector3 move;
	move.x = (fabsf(control.x) > 0.5f) ? control.x*PlayerSpeed : control.x*13.0f/0.5f;
	move.z = control.z;

	// 移動量を反応
	this->rigidbody->velocity.x = move.x;

	// 向きの設定
	this->rigidbody->rotation.y = atan2f(-move.z, move.x) - PI / 2;
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
	this->player->SetAnimation("Idle");
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
	case StateName::SeasonChange:
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
	this->player->SetAnimation("Walk");
	running = false;
}

void Player::StateMove::Update(void)
{
	this->player->anime_timer++;
	auto animset = (ID3DXKeyframedAnimationSet*)this->player->model->GetCurrentAnimation();
	if (this->player->is_grounded)
	{
		if (!this->player->JumpControl())
		{
			float control_len = this->player->control.length();
			if (control_len > 0.0f)
			{
				SetRunning();
				this->player->Move();
				this->player->model->SetAnimationSpeedScale(control_len * (running ? 1.0f : 1.0f/0.5f));
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
	this->player->model->SetAnimationSpeedScale(1.0f);
}

void Player::StateMove::SetState(StateName state)
{
	switch (state)
	{
	case StateName::SeasonChange:
		if (running) break;
	case StateName::Idle:
	case StateName::Air:
	case StateName::Action:
		State::SetState(state);
	}
}

void Player::StateMove::SetRunning(void)
{
	if (running != this->player->control.length() > 0.5f)
	{
		running = !running;
		this->player->SetAnimation(running ? "Run" : "Walk");
	}
}

#pragma endregion

#pragma region StateAction
//=============================================================================
// StateAttack
//=============================================================================
void Player::StateAction::OnEnter(void)
{
	this->player->action_enter();
}

void Player::StateAction::Update(void)
{
	this->player->action_update();
}

void Player::StateAction::OnExit(void)
{
	this->player->action_exit();
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
	this->player->rigidbody->useGravity = true;
	this->player->SetAnimation("Fall");
	if ((this->player->rigidbody->velocity.y > 0.0f) && this->player->jump_timer.TimeUp())
	{
		this->player->jump_sounds[Random(0, this->player->jump_sounds.size() - 1)]->Play();
		this->player->jump_timer.Reset(2.0f);
	}
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
	this->player->PlayFootSound();
	this->player->rigidbody->useGravity = false;
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

#pragma region StateWhistle
void Player::StateSeasonChange::OnEnter(void)
{
	this->player->SetAnimation("SeasonChange", false);
	this->player->rigidbody->velocity.x = 0.0f;
	this->player->skill_sound->Play();
	change = false;
}

void Player::StateSeasonChange::Update(void)
{
	this->player->anime_timer++;

	if (!change && this->player->anime_timer.Elapsed() > 0.8f)
	{
		SeasonManager::SwitchSeason([this]
		{
			this->SetState(StateName::Idle);
		});

		this->player->season_change();

		change = true;
	}
}

void Player::StateSeasonChange::SetState(StateName state)
{
	if(state == StateName::Idle)
		State::SetState(state);
}
#pragma endregion