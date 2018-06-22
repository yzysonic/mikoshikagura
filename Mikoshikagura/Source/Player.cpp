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
	this->anime = AnimeSet::Idle;
	this->is_grounded = false;
	this->is_holding_item = false;

	// モデル初期化
	this->model = AddComponent<SkinnedModel>("player");
	this->model->SetAnime((int)this->anime);

	// コライダー初期化
	this->collider = AddComponent<BoxCollider2D>();
	this->collider->size = Vector2(4.0f, 9.0f);
	this->collider->offset.y += 0.5f*this->collider->size.y;

	// Rigidbody初期化
	this->rigidbody = AddComponent<Rigidbody>();
	this->rigidbody->useGravity = true;

	// 足音初期化
	Foot::player = this;
	this->foot[0] = new Foot("Armature_Foot_L");
	this->foot[1] = new Foot("Armature_Foot_R");
	this->foot_sounds = Sound::GetSerial("foot_mud");
	this->sound_player = AddComponent<SoundPlayer>();

	// ステート初期化
	this->state.resize((int)StateName::Max);
	this->state[(int)StateName::Idle	].reset(new StateIdle(this));
	this->state[(int)StateName::Move	].reset(new StateMove(this));
	this->state[(int)StateName::Air		].reset(new StateAir(this));
	this->state[(int)StateName::Action	].reset(new StateAction(this));
	this->state[(int)StateName::Whistle	].reset(new StateWhistle(this));
	this->current_state = this->state[(int)StateName::Idle].get();

#ifdef _DEBUG
	this->AddComponent<InspectorExtension>(new InspectorContentPlayer);
#endif

}

void Player::Update(void)
{
	MoveControl();
	ActionControl();
	WhistleControl();

	this->current_state->Update();
	this->last_position = this->transform.position;
}

void Player::Uninit(void)
{

}

void Player::OnCollisionEnter(Object * other)
{
	if (other->type == ObjectType::Item && !this->action)
	{
		this->action = [other, this] {
			auto item = other->GetComponent<Holdable>();
			if (!this->is_holding_item)
			{
				item->offset_y = this->collider->size.y +0.5f*item->object->GetComponent<BoxCollider2D>()->size.y + 2.0f;
				item->SetOwner(&this->transform);
				this->is_holding_item = true;
			}
			else
			{
				item->SetOwner(nullptr);
				this->is_holding_item = false;
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
			this->action = nullptr;
		}
	}
	
}

void Player::SetPosition(Vector3 pos)
{
	this->transform.position = pos;
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
	if ((GetKeyboardTrigger(KeyAction) || IsButtonTriggered(ButtonAction)))
	{
		action();
	}
}

void Player::WhistleControl(void)
{
	if ((GetKeyboardTrigger(KeyWhistle) || IsButtonTriggered(ButtonWhistle)) && !this->is_holding_item)
	{
		this->current_state->SetState(StateName::Whistle);
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
	case StateName::Whistle:
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
	this->player->rigidbody->useGravity = true;

	//TODO: ジャンプ／落下のアニメーションにする
	this->player->SetAnime(AnimeSet::Idle);
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
void Player::StateWhistle::OnEnter(void)
{
	SeasonManager::SwitchSeason([this] 
	{
		this->SetState(StateName::Idle); 
	});

	this->player->whistle();
}

void Player::StateWhistle::SetState(StateName state)
{
	if(state == StateName::Idle)
		State::SetState(state);
}
#pragma endregion

#pragma region Foot

Player* Player::Foot::player;

Player::Foot::Foot(const char* name)
{
	this->name = name;
	frame = (BoneFrame*)player->model->FindFrameByName(name);
	collider = AddComponent<BoxCollider2D>();
	collider->size = Vector2::one*0.5f;
	last_ground = nullptr;
	Update();
}

void Player::Foot::Update(void)
{
	D3DXVECTOR4 vec4;
	D3DXVec3Transform(&vec4, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &frame->mtxCombined);
	transform.position.x = vec4.x;
	transform.position.y = vec4.y;
}

void Player::Foot::OnCollisionEnter(Object * other)
{
	if (other->type == ObjectType::Field && !last_ground)
	{
		player->sound_player->SetSound(player->foot_sounds[Random(0, player->foot_sounds.size() - 1)]);
		player->sound_player->Play();
		last_ground = other;
	}
}
void Player::Foot::OnCollisionExit(Object * other)
{
	if (other->type == ObjectType::Field && last_ground == other)
	{
		last_ground = nullptr;
	}
}
#pragma endregion
