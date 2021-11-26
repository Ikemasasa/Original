#pragma once
#include "Singleton.h"

class CameraBase;

/*
	バトルのState設定クラス(要改善
	主にカメラ用
*/
class BattleState : public SingletonManual<BattleState>
{
public:// 定数
	static constexpr float BEGIN_TIME = 1.0f; // 開始演出の時間

	enum class State
	{
		BEGIN,
		BEGIN_FINISHED,
		COMMAND_SELECT,
		PARTY_SELECT,
		ENEMY_SELECT,
		PLAYER_ATTACK,
		ENEMY_ATTACK,
		ESCAPE,
		RESULT,
		MAX
	};

private: // 変数
	State mState = State::BEGIN;	// 現在のステート
	State mOldState = State::BEGIN; // 前回のステート

public: // 関数

	// コンストラクタ
	BattleState() = default;

	// デストラクタ
	~BattleState() = default;

	void SetState(State state) { mOldState = mState; mState = state; }; // ステートを設定
	State GetState() const { return mState; }							// ステートを取得
	State GetOldState() const { return mOldState; }						// 前回のステートを取得
	bool CheckState(State state) { return mState == state; }			// 引数のステートと同じかチェック
	bool CheckOldState(State state) { return mOldState == state; }		// 引数のステートと前回のステートが同じかチェック

};