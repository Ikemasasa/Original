#pragma once
#include <memory>
#include <DirectXMath.h>

#include "lib/Vector.h"

#include "SceneBase.h"

class BattleActorManager;
class CameraBattle;
class Enemy;
class PlayerManager;
class Skybox;
class Terrain;

class SceneBattle : public SceneBase
{
private:
	std::unique_ptr<BattleActorManager> mBattleActorManager = nullptr;
	std::unique_ptr<Skybox>				mSkybox = nullptr;

	DirectX::XMFLOAT4 mLightDir;

public:
	SceneBattle(PlayerManager* plm, Enemy* enemy);
	~SceneBattle();

	void Initialize();
	void Update();
	void Render();
	void Release();
};
