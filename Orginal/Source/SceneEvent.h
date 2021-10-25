#pragma once
#include "SceneBase.h"

#include <memory>

class PlayerManager;
class EnemyManager;
class Terrain;

class SceneEvent : public SceneBase
{
	std::unique_ptr<PlayerManager> mPlayerManager;
	std::unique_ptr<EnemyManager> mEnemyManager;
	std::unique_ptr<Terrain> mTerrain;

public:
	SceneEvent(int eventID, PlayerManager* plm, EnemyManager* em, Terrain* terrain);
	~SceneEvent();

	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;
};