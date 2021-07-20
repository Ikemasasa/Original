#pragma once
#include "SceneBase.h"
#include <DirectXMath.h>
#include <vector>
#include <memory>

class ActorManager;
class CameraBase;
class Enemy;
class Player;
class Skybox;
class Terrain;

class SceneField : public SceneBase
{
	std::unique_ptr<ActorManager> mActorManager = nullptr;
	std::unique_ptr<Skybox>		  mSkybox		= nullptr;

	void RenderLight();
public:
	SceneField();
	~SceneField();

	void Initialize();
	void Update();
	void Render();
	void Release();
};