#pragma once
#include <memory>
#include <DirectXMath.h>

#include "lib/Vector.h"

#include "Music.h"
#include "SceneBase.h"

class BattleCharacterManager;
class Enemy;
class PlayerManager;
class SelectOptions;
class Skybox;
class Terrain;
class TurnManager;

class SceneBattle : public SceneBase
{
	static constexpr float OPTIONS_X = 100;
	static constexpr float OPTIONS_Y = 220;

public:
	enum Result
	{
		NONE,
		PLAYER_WIN,
		PLAYER_LOSE,
	};

private:
	std::unique_ptr<BattleCharacterManager> mBattleCharacterManager = nullptr;
	std::unique_ptr<Skybox>					mSkybox = nullptr;
	std::shared_ptr<Terrain>				mTerrain = nullptr;
	std::unique_ptr<TurnManager>			mTurnManager = nullptr;
	std::unique_ptr<SelectOptions>			mSelectOptions = nullptr;

	PlayerManager* mPlayerManager;
	Enemy* mHitEnemy;


	Result mResult;
	Music::Kind mBattleMusic;
	Music::Kind mResultMusic;

public:
	SceneBattle(PlayerManager* plm, Enemy* enemy);
	~SceneBattle();

	void Initialize();
	void Update();
	void Render();
	void Release();
};
