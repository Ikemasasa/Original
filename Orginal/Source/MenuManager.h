#pragma once
#include <memory>
#include <stack>

#include "MenuSelect.h"
#include "MenuItem.h"

class Sprite;
class PlayerManager;

class MenuManager
{
	std::unique_ptr<Sprite> mBG;
	std::stack<std::unique_ptr<MenuBase>> mMenuStack;
	std::unique_ptr<MenuBase> mNextMenu;

	MenuBase::Select mNextState;

	std::unique_ptr<MenuItem> mMenuItem;

public:
	MenuManager();

	void Initialize(const PlayerManager* plm);
	void Update(PlayerManager* plm);
	void Render();
	void Release();
};