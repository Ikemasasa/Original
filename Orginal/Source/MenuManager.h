#pragma once
#include <memory>
#include <stack>

#include "MenuSelect.h"
#include "MenuItem.h"

class Texture;
class PlayerManager;

class MenuManager
{
	std::unique_ptr<Texture> mBG;
	std::stack<std::unique_ptr<MenuBase>> mMenuStack;
	std::unique_ptr<MenuBase> mNextMenu;

	MenuSelect mMenuSelect;
	MenuBase::Select mState;

	std::unique_ptr<MenuItem> mMenuItem;

public:
	MenuManager();

	void Initialize();
	void Update(PlayerManager* plm);
	void Render();
	void Release();
};