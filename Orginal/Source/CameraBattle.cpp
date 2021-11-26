#include "CameraBattle.h"

#include "BattleCharacter.h"
#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CommandBase.h"
#include "Fade.h"
#include "GameManager.h"

CameraBattle::CameraBattle() : CameraBase()
{

}

void CameraBattle::Initialize(const Vector3& target, float distance)
{
	// 基底クラスの初期化関数
	CameraBase::Initialize(target, distance);

	// 引数からターゲット設定
	mTarget = target;

	// 距離、天頂角を定数から設定
	mDistance = distance;
	mZenithAngle = BEGIN_START_ZENITH;
	mAzimuthAngle = BEGIN_START_AZIMUTH;

	// 座標を設定
	mPos = CalcPosFromAngle(distance);
	mBeginPos = mPos;
	mEndPos = CalcPosFromAngle(BEGIN_END_DISTANCE);

	// ビュー更新
	UpdateView();
}

void CameraBattle::Update(const BattleCharacter* commandChara, const BattleCharacterManager* bcm)
{
	// 攻撃終了フラグをリセット
	ResetAttackFinishFlag();

	// BattleStateによって分岐
	BattleState::State state = BattleState::GetInstance().GetState();

	switch (state)
	{
	case BattleState::State::BEGIN:			 Begin(commandChara); break;
	case BattleState::State::COMMAND_SELECT: CommandSelect(commandChara); break;
	case BattleState::State::PARTY_SELECT:   PartySelect(commandChara); break;
	case BattleState::State::ENEMY_SELECT:   EnemySelect(commandChara); break;
	case BattleState::State::PLAYER_ATTACK:  PlayerAttack(commandChara, bcm); break;
	case BattleState::State::ENEMY_ATTACK:	 EnemyAttack(commandChara, bcm); break;
	case BattleState::State::ESCAPE:		 Escape(commandChara); break;
	}

	// 振動、ビューを更新
	UpdateShake(GameManager::elapsedTime);
	UpdateView();
}

void CameraBattle::Begin(const BattleCharacter* commandChara)
{	
	// フェードアウトのa値がLERP_START_FADE_ALPHAより大きいならreturn
	if (!Fade::GetInstance().CheckFadeState(Fade::FADE_OUT, LERP_START_FADE_ALPHA)) return;

	// 座標を線形補完で更新
	mPos = Vector3::Lerp(mBeginPos, mEndPos, mLerpFactor);

	// 係数が1未満なら係数を加算する
	if (mLerpFactor < 1.0f)
	{
		mLerpVelocity += BEGIN_LERP_ACCEL;
		mLerpFactor += mLerpVelocity;
	}
	else
	{
		// 1以上(移動が終了)ならタイマーを加算
		mTimer += GameManager::elapsedTime;
	}

	// タイマーがBattleState::BEGIN_TIMEを超えたら開始演出終了
	if (mTimer >= BattleState::BEGIN_TIME)
	{
		// BattleStateを開始演出終了にする
		BattleState::GetInstance().SetState(BattleState::State::BEGIN_FINISHED);


		// mCommandSelectPosに初期値を入れる

		// コマンドキャラの情報を取得
		Vector3 charaPos = commandChara->GetPos(); 
		float radius = commandChara->GetCapsule().radius;
		float diameter = radius * 2.0f;
		Vector3 front = commandChara->GetFrontXZ();
		Vector3 right = commandChara->GetRightXZ();

		// キャラの座標によって左右に分ける
		if (charaPos.x < 0) right = -right;

		// 座標決定
		mCommandSelectPos.x = charaPos.x + (front.x * diameter * COMMAND_SELECT_DIST_XZ) + (right.x * radius * COMMAND_SELECT_DIST_XZ);
		mCommandSelectPos.y = charaPos.y + commandChara->GetHeight() * COMMAND_SELECT_HEIGHT_RATE;
		mCommandSelectPos.z = charaPos.z + (front.z * diameter * COMMAND_SELECT_DIST_XZ) + (right.z * radius * COMMAND_SELECT_DIST_XZ);
	}
}

void CameraBattle::CommandSelect(const BattleCharacter* commandChara)
{
	// コマンドキャラの情報を取得
	Vector3 charaPos = commandChara->GetPos();
	float radius = commandChara->GetCapsule().radius;
	float diameter = radius * 2.0f;
	Vector3 front = commandChara->GetFrontXZ();
	Vector3 right = commandChara->GetRightXZ();

	// キャラの座標によって左右に分ける
	if (charaPos.x < 0) right = -right;

	// 座標算出
	Vector3 pos = {};
	pos.x = charaPos.x + (front.x * diameter * COMMAND_SELECT_DIST_XZ) + (right.x * radius * COMMAND_SELECT_DIST_XZ);
	pos.y = charaPos.y + commandChara->GetHeight() * COMMAND_SELECT_HEIGHT_RATE;
	pos.z = charaPos.z + (front.z * diameter * COMMAND_SELECT_DIST_XZ) + (right.z * radius * COMMAND_SELECT_DIST_XZ);

	// コマンドセレクト座標に設定
	mCommandSelectPos = pos;

	// 座標を線形補完で設定
	mPos = Vector3::Lerp(mPos, pos, LERP_SPEED);
	
	// ターゲットを敵の座標の真ん中に設定
	Vector3 target = Vector3(0.0f, 0.0f, BattleCharacterManager::ENEMY_POS_Z);
	mTarget = Vector3::Lerp(mTarget, target, LERP_SPEED);
}

void CameraBattle::PartySelect(const BattleCharacter* commandChara)
{
	// 座標をコマンドセレクトのZ座標反転に設定
	mPos = Vector3::Lerp(mPos, Vector3(mCommandSelectPos.x, mCommandSelectPos.y, -mCommandSelectPos.z), LERP_SPEED);

	// ターゲットをプレイヤーの座標の真ん中に設定
	mTarget = Vector3::Lerp(mTarget, Vector3(0.0f, 0.0f, BattleCharacterManager::PLAYER_POS_Z), LERP_SPEED);
}

void CameraBattle::EnemySelect(const BattleCharacter* commandChara)
{
	// 座標をコマンドセレクト座標に設定
	mPos = Vector3::Lerp(mPos, mCommandSelectPos, LERP_SPEED);

	// ターゲットを敵の座標の真ん中に設定
	mTarget = Vector3::Lerp(mTarget, Vector3(0.0f, 0.0f, BattleCharacterManager::ENEMY_POS_Z), LERP_SPEED);
}

void CameraBattle::PlayerAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm)
{	
	// コマンドキャラのモーションを取得
	Character::MotionType oldMotion = mCommandCharaMotion;
	mCommandCharaMotion = commandChara->GetMotion();

	// モーションが、前回が攻撃かつ、今回攻撃じゃないならAttackfinishflagをon
	if (oldMotion == Character::ATTACK && mCommandCharaMotion != Character::ATTACK)
	{
		mPlAttackFinishFlag = true;
	}

	// 座標を算出する
	Vector3 pos = {};
	if (mPlAttackFinishFlag)
	{
		// 攻撃が終わっていたらコマンド選択の座標
		pos = mCommandSelectPos;
	}
	else
	{
		// 攻撃が終わっていなかったら
		// プレイヤーの情報
		Vector3 charaPos = commandChara->GetPos();
		float radius = commandChara->GetCapsule().radius;
		float diameter = radius * 2.0f;
		Vector3 front = commandChara->GetFrontXZ();
		Vector3 right = commandChara->GetRightXZ();
		if (charaPos.x < 0) right = -right;

		// 座標を算出
		pos.x = charaPos.x + (front.x * radius * ATTACK_DISTANCE) + (right.x * diameter * ATTACK_DISTANCE);
		pos.y = charaPos.y + commandChara->GetHeight() * ATTACK_HEIGHT_RATE;
		pos.z = charaPos.z + (front.z * radius * ATTACK_DISTANCE) + (right.z * diameter * ATTACK_DISTANCE);

	}
	// 座標を更新
	mPos = Vector3::Lerp(mPos, pos, LERP_SPEED);



	// ターゲット決定
	Vector3 target = {  };
	if (commandChara->GetMotion() == Character::ATTACK)
	{
		// 攻撃モーション中なら、攻撃相手の平均座標をターゲットに

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
			target = Vector3::ZERO;
		}
		else
		{
			target = commandChara->GetTargetPos();
		}

	}
	// ターゲットを更新
	mTarget = Vector3::Lerp(mTarget, target, LERP_SPEED);
}

void CameraBattle::EnemyAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm)
{
	// コマンドキャラのモーションを取得
	Character::MotionType oldMotion = mCommandCharaMotion;
	mCommandCharaMotion = commandChara->GetMotion();

	// モーションが、前回が攻撃かつ、今回攻撃じゃないならAttackfinishflagをon
	if (oldMotion == Character::ATTACK && mCommandCharaMotion != Character::ATTACK)
	{
		mEnmAttackFinishFlag = true;
	}

	// 座標を算出する
	Vector3 pos = {};
	if (mEnmAttackFinishFlag)
	{
		// 攻撃が終わっていたらコマンド選択の座標
		pos = mCommandSelectPos;
	}
	else
	{
		// プレイヤーの情報
		Vector3 charaPos = commandChara->GetPos();
		float radius = commandChara->GetCapsule().radius;
		float diameter = radius * 2.0f;
		Vector3 front = commandChara->GetFrontXZ();
		Vector3 right = commandChara->GetRightXZ();
		if (charaPos.x < 0) right = -right;

		// 座標を算出
		pos.x = charaPos.x + (front.x * radius * ATTACK_DISTANCE) + (right.x * diameter * ATTACK_DISTANCE);
		pos.y = charaPos.y + commandChara->GetHeight() * ATTACK_HEIGHT_RATE;
		pos.z = charaPos.z + (front.z * radius * ATTACK_DISTANCE) + (right.z * diameter * ATTACK_DISTANCE);

	}

	// 座標を更新
	mPos = Vector3::Lerp(mPos, pos, LERP_SPEED);


	// ターゲット決定
	Vector3 target = {  };
	if (commandChara->GetMotion() == Character::ATTACK)
	{
		// 攻撃モーション中なら、攻撃相手の平均座標をターゲットに

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
			target = Vector3::ZERO;
		}
		else
		{
			target = commandChara->GetTargetPos();
		}
	}
	// ターゲットを更新
	mTarget = Vector3::Lerp(mTarget, target, LERP_SPEED);
}

void CameraBattle::Escape(const BattleCharacter* commandChara)
{
	// 適当に設定
	mPos = Vector3::Lerp(mPos, mCommandSelectPos, LERP_SPEED);
	mTarget = Vector3::Lerp(mTarget, commandChara->GetTargetPos(), LERP_SPEED);
}

void CameraBattle::ResetAttackFinishFlag()
{
	BattleState::State state = BattleState::GetInstance().GetState();
	BattleState::State old = BattleState::GetInstance().GetOldState();

	if (old == BattleState::State::PLAYER_ATTACK && state != BattleState::State::PLAYER_ATTACK)
	{
		mPlAttackFinishFlag = false;
	}

	if (old == BattleState::State::ENEMY_ATTACK && state != BattleState::State::ENEMY_ATTACK)
	{
		mEnmAttackFinishFlag = false;
	}
}
