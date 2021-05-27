#pragma once
#include <memory>

#include "MenuSelect.h"
#include "MenuItem.h"

class Texture;
class PlayerManager;

class MenuManager
{
	std::unique_ptr<Texture> mBG;

	MenuSelect mMenuSelect;
	MenuSelect::Select mState;

	std::unique_ptr<MenuItem> mMenuItem;

public:
	MenuManager();

	void Initialize();
	void Update(PlayerManager* plm);
	void Render();
	void Release();
};