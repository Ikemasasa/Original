#include "CameraBattle.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CommandBase.h"
#include "Character.h"
#include "Define.h"
#include "Fade.h"
#include "GameManager.h"

CameraBattle::CameraBattle() : CameraBase()
{
	// 開始演出の座標決定
	const float BEGIN_POS_Y = 10.0f;
	mBeginPos.x = 0.0f;
	mBeginPos.y = BEGIN_POS_Y;
	mBeginPos.z = BattleCharacterManager::PLAYER_POS_Z * 2.0f;

	Vector3 dist = Vector3(0, 0, BattleCharacterManager::ENEMY_POS_Z) - mBeginPos;
	float distRate = 0.3f;
	mEndPos = mBeginPos + dist * distRate;
	
	const float BEGIN_TARGET_Y = 3.0f;
	mTarget.x = 0.0f;
	mTarget.y = BEGIN_TARGET_Y;
	mTarget.z = BattleCharacterManager::ENEMY_POS_Z;
}

void CameraBattle::Update(const BattleCharacter* commandChara, const BattleCharacterManager* bcm)
{
	BattleState::State state = BattleState::GetInstance().GetState();
	if (state != BattleState::State::PLAYER_ATTACK) mPlAttackFinishFlag = false;
	if (state != BattleState::State::ENEMY_ATTACK) mEnmAttackFinishFlag = false;

	switch (state)
	{
	case BattleState::State::BEGIN:
		Begin();
		if (mTimer >= BattleState::BEGIN_TIME)
		{
			BattleState::GetInstance().SetState(BattleState::State::BEGIN_FINISHED);
		}

		break;

	case BattleState::State::COMMAND_SELECT: CommandSelect(commandChara); break;
	case BattleState::State::PARTY_SELECT:   PartySelect(commandChara); break;
	case BattleState::State::ENEMY_SELECT:   EnemySelect(commandChara); break;
	case BattleState::State::PLAYER_ATTACK:  PlayerAttack(commandChara, bcm); break;
	case BattleState::State::ENEMY_ATTACK:	 EnemyAttack(commandChara, bcm); break;
	case BattleState::State::ESCAPE:		 Escape(commandChara); break;
	}

	UpdateShake(GameManager::elapsedTime);
	UpdateView();
}

void CameraBattle::Begin()
{
	mPos = Vector3::Lerp(mBeginPos, mEndPos, mLerpFactor);

	if (!Fade::GetInstance().CheckFadeState(Fade::FADE_OUT, LERP_START_FADE_ALPHA)) return;

	if (mLerpFactor < 1.0f)
	{
		const float LERP_ACCEL = 0.001f;
		mLerpVelocity += LERP_ACCEL;
		mLerpFactor += mLerpVelocity;
	}
	else
	{
		mTimer += GameManager::elapsedTime;
	}
}

void CameraBattle::CommandSelect(const BattleCharacter* commandChara)
{
	// 定数
	const float POS_Y_RATE = 2.0f;
	
	Vector3 charaPos = commandChara->GetPos();

	// 座標決定
	Vector3 pos = {};
	Vector3 direction = {};
	float diameter = commandChara->GetCapsule().radius * 2.0f;
	Vector3 front = commandChara->GetFrontXZ();
	Vector3 right = commandChara->GetRightXZ();
	if (commandChara->GetPos().x < 0) right = -right;

	pos.x = charaPos.x + (front.x * diameter * COMMAND_SELECT_DIST) + (right.x * diameter * COMMAND_SELECT_DIST * 0.5f);
	pos.y = charaPos.y + commandChara->GetHeight() * POS_Y_RATE;
	pos.z = charaPos.z + (front.z * diameter * COMMAND_SELECT_DIST) + (right.z * diameter * COMMAND_SELECT_DIST * 0.5f);


	mCommandSelectPos = pos;
	mPos = Vector3::Lerp(mPos, pos, LERP_SPEED);
	
	Vector3 target = Vector3(0.0f, 0.0f, BattleCharacterManager::ENEMY_POS_Z);
	mTarget = Vector3::Lerp(mTarget, target, LERP_SPEED);
}

void CameraBattle::PartySelect(const BattleCharacter* commandChara)
{
	mPos = Vector3::Lerp(mPos, Vector3(mCommandSelectPos.x, mCommandSelectPos.y, -mCommandSelectPos.z), LERP_SPEED);
	mTarget = Vector3::Lerp(mTarget, Vector3(0.0f, 0.0f, BattleCharacterManager::PLAYER_POS_Z), LERP_SPEED);
}

void CameraBattle::EnemySelect(const BattleCharacter* commandChara)
{
	mPos = Vector3::Lerp(mPos, mCommandSelectPos, LERP_SPEED);
	mTarget = Vector3::Lerp(mTarget, Vector3(0.0f, 0.0f, BattleCharacterManager::ENEMY_POS_Z), LERP_SPEED);
}

void CameraBattle::PlayerAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm)
{
	// 定数
	const float POS_Y_RATE = 0.5f;
	static Character::MotionType oldMotion = commandChara->GetMotion();

	// プレイヤーの情報
	Vector3 charaPos = commandChara->GetPos();
	float charaHeight = commandChara->GetHeight();
	float diameter = commandChara->GetCapsule().radius * 2.0f;

	if (oldMotion == Character::ATTACK)
	{
		// 攻撃終わったら
		if (commandChara->GetMotion() != Character::ATTACK)
		{
			mPlAttackFinishFlag = true;
		}
	}

	// 座標
	Vector3 pos = {};
	if (!mPlAttackFinishFlag)
	{
		Vector3 front = commandChara->GetFrontXZ();
		Vector3 right = commandChara->GetRightXZ();
		if (charaPos.x < 0) right = -right;

		pos.x = charaPos.x + (front.x * diameter * ATTACK_DIST * 0.5f) + (right.x * diameter * ATTACK_DIST);
		pos.y = charaPos.y + commandChara->GetHeight() * POS_Y_RATE;
		pos.z = charaPos.z + (front.z * diameter * ATTACK_DIST * 0.5f) + (right.z * diameter * ATTACK_DIST);
	}
	else
	{
		pos = mCommandSelectPos;
	}

	// ターゲット決定
	Vector3 target = {  };
	// 攻撃モーション中なら
	if (commandChara->GetMotion() == Character::ATTACK)
	{
		// 敵にターゲット

		// ID取得
		std::vector<int> targetIds = commandChara->GetCommand()->GetTargetObjIDs();

		// 平均の座標を求める
		for (const auto& id : targetIds)
		{
			BattleCharacter* bc = bcm->GetChara(id);
			target += bc->GetTargetPos();
		}
		target /= targetIds.size();
	}
	else
	{
		if (mPlAttackFinishFlag)
		{
			target = Vector3(0.0f, 0.0f, BattleCharacterManager::ENEMY_POS_Z);
		}
		else
		{
			// 自分にターゲット
			target = commandChara->GetTargetPos();
		}
	}

	// 値を決定
	mPos = Vector3::Lerp(mPos, pos, LERP_SPEED);
	mTarget = Vector3::Lerp(mTarget, target, LERP_SPEED);

	// モーション保存
	oldMotion = commandChara->GetMotion();
}

void CameraBattle::EnemyAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm)
{
	const float POS_Y_RATE = 0.5f;
	static Character::MotionType oldMotion = commandChara->GetMotion();

	// プレイヤーの情報
	Vector3 charaPos = commandChara->GetPos();
	float charaHeight = commandChara->GetHeight();
	float diameter = commandChara->GetCapsule().radius * 2.0f;

	if (oldMotion == Character::ATTACK)
	{
		// 攻撃終わったら
		if (commandChara->GetMotion() != Character::ATTACK)
		{
			mEnmAttackFinishFlag = true;
		}
	}

	// 座標
	Vector3 pos = {};
	if (!mEnmAttackFinishFlag)
	{
		Vector3 front = commandChara->GetFrontXZ();
		Vector3 right = commandChara->GetRightXZ();
		if (charaPos.x < 0) right = -right;

		pos.x = charaPos.x + (front.x * diameter * ATTACK_DIST * 0.5f) + (right.x * diameter * ATTACK_DIST);
		pos.y = charaPos.y + commandChara->GetHeight() * POS_Y_RATE;
		pos.z = charaPos.z + (front.z * diameter * ATTACK_DIST * 0.5f) + (right.z * diameter * ATTACK_DIST);
	}
	else
	{
		pos = mCommandSelectPos;
	}

	// ターゲット決定
	Vector3 target = {  };
	// 攻撃モーション中なら
	if (commandChara->GetMotion() == Character::ATTACK)
	{
		// 敵にターゲット

		// ID取得
		std::vector<int> targetIds = commandChara->GetCommand()->GetTargetObjIDs();

		// 平均の座標を求める
		for (const auto& id : targetIds)
		{
			BattleCharacter* bc = bcm->GetChara(id);
			target += bc->GetTargetPos();
		}
		target /= targetIds.size();
	}
	else
	{
		if (mEnmAttackFinishFlag)
		{
			target = Vector3(0.0f, 0.0f, BattleCharacterManager::ENEMY_POS_Z);
		}
		else
		{
			// 自分にターゲット
			target = commandChara->GetTargetPos();
		}
	}

	// 値を決定
	mPos = Vector3::Lerp(mPos, pos, LERP_SPEED);
	mTarget = Vector3::Lerp(mTarget, target, LERP_SPEED);

	// モーション保存
	oldMotion = commandChara->GetMotion();
}

void CameraBattle::Escape(const BattleCharacter* commandChara)
{
	mPos = Vector3::Lerp(mPos, mCommandSelectPos, LERP_SPEED);
	mTarget = Vector3::Lerp(mTarget, commandChara->GetTargetPos(), LERP_SPEED);
}
