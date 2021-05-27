#pragma once
#include <memory>
#include <DirectXMath.h>

#include "lib/Vector.h"

#include "Enemy.h"
#include "SceneBase.h"
#include "Player.h"

class BattleActorManager;
class CameraBattle;
class Skybox;
class Terrain;

class SceneBattle : public SceneBase
{
private:
	std::unique_ptr<BattleActorManager> mBattleActorManager = nullptr;
	std::unique_ptr<Skybox>				mSkybox				= nullptr;

	DirectX::XMFLOAT4 mLightDir;

public:
	SceneBattle(const std::shared_ptr<Player>& player, const std::shared_ptr<Enemy>& enemy);
	~SceneBattle();

	void Initialize();
	void Update();
	void Render();
	void Release();
};
