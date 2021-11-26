#pragma once
#include "CameraBase.h"

#include <memory>

#include "lib/Vector.h"

#include "Character.h"

/*
	SceneBattleに使うカメラ
	BattleStateクラスを参照してるから結構いかついコードになってる
*/
class CameraBattle : public CameraBase
{
	// 定数

	// 共通
	static constexpr float LERP_SPEED = 0.4f;

	// 戦闘開始演出関係
public:
	static constexpr float BEGIN_START_DISTANCE = 16.0f; // 戦闘開始演出が開始する時のターゲットからの距離
private:
	static constexpr float BEGIN_END_DISTANCE = 10.0f;    // 戦闘開始演出が終了する時のターゲットからの距離
	static constexpr float BEGIN_START_ZENITH = 70 * 0.01745f;   // 戦闘開始演出が開始するときの天頂角(Rad)
	static constexpr float BEGIN_START_AZIMUTH = -90 * 0.01745f; // 戦闘開始演出が開始するときの方位角(Rad)
	static constexpr float LERP_START_FADE_ALPHA = 0.6f;   // 戦闘開始演出が開始するフェードのA値
	static constexpr float BEGIN_LERP_ACCEL = 0.001f;      // 戦闘開始演出の線形補完の加速度

	// コマンドセレクト関係
	static constexpr float COMMAND_SELECT_DIST_XZ = 7.0f;	  // コマンドセレクトのXZの距離
	static constexpr float COMMAND_SELECT_HEIGHT_RATE = 2.0f; // コマンドセレクトの高さの割合

	// 攻撃関係
	static constexpr float ATTACK_DISTANCE = 6.0f;		  // 攻撃のターゲットからの距離
	static constexpr float ATTACK_HEIGHT_RATE = 0.5f; // 攻撃するキャラの高さの割合


private: //変数

	// 共通
	float mDistance = 0.0f;   // ターゲットからの距離
	float mLerpFactor = 0.0f; // 線形補完の係数
	Vector3 mCommandSelectPos = {}; // コマンド選択の時の座標(攻撃後にこの位置に戻す)

	// 戦闘開始演出関係
	float mTimer = 0.0f;		// 演出の時間管理のタイマー
	float mLerpVelocity = 0.0f; // 線形補完の係数の加速値
	Vector3 mBeginPos = {};		// 戦闘開始演出の開始位置
	Vector3 mEndPos = {};		// 戦闘開始演出の終了位置

	// 攻撃関係
	Character::MotionType mCommandCharaMotion = Character::IDLE; // 現在行動中のキャラのモーション
	bool mPlAttackFinishFlag = false;							 // 攻撃モーションが終わったかのフラグ
	bool mEnmAttackFinishFlag = false;							 // 攻撃モーションが終わったかのフラグ

private: // 関数
	
	// 戦闘開始演出のカメラ
	void Begin(const BattleCharacter* commandChara);

	// コマンドセレクト中のカメラ
	void CommandSelect(const BattleCharacter* commandChara);

	// パーティーキャラを選ぶときのカメラ
	void PartySelect(const BattleCharacter* commandChara);

	// 敵キャラを選ぶときのカメラ
	void EnemySelect(const BattleCharacter* commandChara);

	// プレイヤーが攻撃するときのカメラ
	void PlayerAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm);

	// 敵が攻撃するときのカメラ
	void EnemyAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm);

	// 逃げるときのカメラ
	void Escape(const BattleCharacter* commandChara);

	// 攻撃終了フラグをリセット
	void ResetAttackFinishFlag();

public:

	// コンストラクタ
	CameraBattle();
	
	// デストラクタ
	~CameraBattle() = default;

	// 初期化関数(オーバーライド)
	void Initialize(const Vector3& target, float distance = BEGIN_START_DISTANCE) override;

	// 更新関数(オーバーライド)
	void Update(const BattleCharacter* commandChara, const BattleCharacterManager* bcm) override;
};