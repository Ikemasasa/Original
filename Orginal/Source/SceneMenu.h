#pragma once
#include "SceneBase.h"
#include "MenuManager.h"

class Sprite;
class PlayerManager;

class SceneMenu : public SceneBase
{
	PlayerManager* mPlayerManager;
	MenuManager mMenuManager;

public:
	SceneMenu(PlayerManager* plm);
	~SceneMenu();

	void Initialize();
	void Update();
	void Render();
	void Release();
};