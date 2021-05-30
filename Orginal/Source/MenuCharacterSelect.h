#pragma once
#include<memory>

#include "lib/Font.h"

class PlayerManager;
class Texture;

class MenuCharacterSelect
{
	static constexpr float PLATE_OFFSET_X = 100.0f;
	static constexpr float PLATE_OFFSET_Y = 50.0f;

	std::unique_ptr<Texture> mPlate;
	std::unique_ptr<Texture> mPlateSelect;

	int mSelectIndex;
	Font mFont;

public:
	MenuCharacterSelect(PlayerManager* plm);
	~MenuCharacterSelect();

	void Update();
	void Render();

	int GetSelectIndex() const { return mSelectIndex; }
	void ResetSelectIndex() { mSelectIndex = 0; }
};