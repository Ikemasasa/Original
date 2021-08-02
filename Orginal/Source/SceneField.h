#pragma once
#include "SceneBase.h"
#include <DirectXMath.h>
#include <vector>
#include <memory>

class CharacterManager;
class CameraBase;
class Enemy;
class Player;
class Skybox;
class Terrain;

class SceneField : public SceneBase
{
	std::unique_ptr<CharacterManager> mCharaManager = nullptr;
	std::unique_ptr<Terrain>	      mTerrain		= nullptr;
	std::unique_ptr<Skybox>			  mSkybox		= nullptr;

public:
	SceneField();
	~SceneField();

	void Initialize();
	void Update();
	void Render();
	void Release();
};