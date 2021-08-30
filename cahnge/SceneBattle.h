#pragma once
#include <memory>
#include <DirectXMath.h>

#include "lib/Audio.h"
#include "lib/Vector.h"

#include "SceneBase.h"

class BattleCharacterManager;
class Enemy;
class PlayerManager;
class Skybox;
class Terrain;
class TurnManager;

class SceneBattle : public SceneBase
{
public:
	enum Result
	{
		NONE,
		PLAYER_WIN,
		PLAYER_LOSE,
	};

private:
	std::unique_ptr<BattleCharacterManager> mBattleCharacterManager = nullptr;
	std::unique_ptr<Skybox>				mSkybox = nullptr;
	std::shared_ptr<Terrain> mTerrain = nullptr;
	std::unique_ptr<TurnManager> mTurnManager;

	Enemy* mHitEnemy = nullptr;    // fieldで当たった敵
	PlayerManager* mPlayerManager; // ドロップアイテムを格納するため
	Font mFont;
	Result mResult;
	Music music;
	Music result;

public:
	SceneBattle(PlayerManager* plm, Enemy* enemy);
	~SceneBattle();

	void Initialize();
	void Update();
	void Render();
	void Release();
};
