#pragma once
#include <DirectXMath.h>
#include <memory>
#include <queue>
#include <vector>

#include "BattleCharacter.h"
#include "CharacterHealth.h"
#include "TurnManager.h"

class Enemy;
class Player;
class PlayerManager;

// ターン進行の部分を後々別にしたい

class BattleCharacterManager
{
	enum Result
	{
		NONE,
		PLAYER_WIN,
		PLAYER_LOSE,
	};


	static constexpr float HEALTH_PLATE_X = 900.0f;
	static constexpr float HEALTH_PLATE_Y = 0.0f;

	static const int BATTLECHARA_MAX = 12;
	static const int BATTLECHARA_KIND = 2; // Player, Enemyの2つ 

public:
	static constexpr float ENEMY_POS_Z = 5.0f;
	static constexpr float PLAYER_POS_Z = -5.0f;

	static constexpr float POS_MIN_X = -5.0f;
	static constexpr float POS_MAX_X =  5.0f;


private:
	std::vector<std::shared_ptr<BattleCharacter>> mBCharacters;
	std::vector<int> mAliveCharaIDs[BATTLECHARA_KIND];
	int mPlayerNum;

	CharacterHealth mCharacterHealth;
	TurnManager mTurnManager;

	Enemy* mHitEnemy = nullptr; // fieldで当たった敵

	Result CheckBattleFinish();
	void OrganizeCharacter();

	void PlayerCreateAndRegister(Player* pl);
	void EnemyCreateAndRegister(Enemy* enm);
public:	
	BattleCharacterManager(PlayerManager* player, Enemy* enemy);
	~BattleCharacterManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);

	// ゲッター
	BattleCharacter* GetMoveChara() const { return mTurnManager.GetMoveChara(); }
	const std::vector<int>& GetAliveCharaIDs(Character::Type kind) const { return mAliveCharaIDs[kind]; }
	BattleCharacter* GetChara(int objectID) const { return mBCharacters[objectID].get(); }
	const std::vector<std::shared_ptr<BattleCharacter>>& GetBCharacters() const { return mBCharacters; }

};