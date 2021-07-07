#pragma once
#include <DirectXMath.h>
#include <memory>
#include <queue>
#include <vector>

#include "Actor.h"
#include "BattleActor.h"
#include "CharacterHealth.h"
#include "TurnManager.h"

class Enemy;
class Player;
class PlayerManager;

// �^�[���i�s�̕�������X�ʂɂ�����

class BattleActorManager
{
	enum Result
	{
		NONE,
		PLAYER_WIN,
		PLAYER_LOSE,
	};


	static constexpr float HEALTH_PLATE_X = 900.0f;
	static constexpr float HEALTH_PLATE_Y = 0.0f;

	static const int BATTLEACTOR_MAX = 12;
	static const int BATTLEACTOR_KIND = 2; // Player, Enemy��2�� 

public:
	static constexpr float ENEMY_POS_Z = 5.0f;
	static constexpr float PLAYER_POS_Z = -5.0f;


private:
	std::vector<std::shared_ptr<BattleActor>> mBActors;
	std::vector<int> mAliveActorIDs[BATTLEACTOR_KIND];
	int mPlayerNum;

	CharacterHealth mCharacterHealth;
	TurnManager mTurnManager;

	Enemy* mHitEnemy = nullptr; // field�œ��������G

	Result CheckBattleFinish();
	void OrganizeActor();
	void PlayDeathEffect();

	void PlayerCreateAndRegister(Player* pl);
	void EnemyCreateAndRegister(Enemy* enm);
public:	
	BattleActorManager(PlayerManager* player, Enemy* enemy);
	~BattleActorManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);

	// �Q�b�^�[
	BattleActor* GetMoveActor() const { return mTurnManager.GetMoveActor(); }
	const std::vector<int>& GetAliveActorIDs(Actor::Type kind) const { return mAliveActorIDs[kind]; }
	BattleActor* GetActor(int objectID) const { return mBActors[objectID].get(); }
	const std::vector<std::shared_ptr<BattleActor>>& GetBActors() const { return mBActors; }

};