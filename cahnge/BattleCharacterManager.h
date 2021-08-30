#pragma once
#include <DirectXMath.h>
#include <memory>
#include <queue>
#include <vector>

#include "BattleCharacter.h"
#include "CharacterHealth.h"
#include "DropItemShower.h"
#include "TurnManager.h"
#include "SceneBattle.h"

class Enemy;
class Player;
class PlayerManager;

// ターン進行の部分を後々別にしたい

class BattleCharacterManager
{


	static constexpr float HEALTH_BOARD_X = 900.0f;
	static constexpr float HEALTH_BOARD_Y = 0.0f;

	static const int BATTLECHARA_MAX = 12;
	static const int BATTLECHARA_KIND = 2; // Player, Enemyの2つ 

public:
	static constexpr float ENEMY_POS_Z = 5.0f;
	static constexpr float PLAYER_POS_Z = -5.0f;

	static constexpr float POS_MIN_X = -5.0f;
	static constexpr float POS_MAX_X =  5.0f;


private:
	std::vector<std::shared_ptr<BattleCharacter>> mBCharacters;
	std::vector<int> mAliveObjIDs[BATTLECHARA_KIND];
	std::vector<int> mDropItemIDs;
	BattleCharacter* mMoveChara;
	int mPlayerNum;

	CharacterHealth mCharacterHealth;
	DropItemShower mDropItemShower;

	void PlayerCreateAndRegister(Player* pl);
	void EnemyCreateAndRegister(Enemy* enm);
public:	
	BattleCharacterManager(PlayerManager* pm, Enemy* enemy);
	~BattleCharacterManager() = default;

	void Initialize();
	void Update(const TurnManager* turnManager);
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);

	// その他関数
	void OrganizeCharacter(); // 体力が0になったキャラをmAliveObjIDsから消す
	SceneBattle::Result CheckBattleFinish(); // mAliveObjIDsから、バトル終了の判定をする

	// ゲッター
	BattleCharacter* GetMoveChara() const { return mMoveChara; }
	const std::vector<int>& GetAliveObjIDs(Character::Type kind) const { return mAliveObjIDs[kind]; }
	BattleCharacter* GetChara(int objectID) const { return mBCharacters[objectID].get(); }
	const std::vector<std::shared_ptr<BattleCharacter>>& GetBCharacters() const { return mBCharacters; }
	const std::vector<int>& GetDropItemIDs() const { return mDropItemIDs; }
};