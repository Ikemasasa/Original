#pragma once
#include <memory>

#include "lib/Font.h"

class PlayerManager;
class Texture;

class MenuCharacterSelect
{
	std::unique_ptr<Texture> mPlate;
	int mCharacterNum = 0;
	int mSelectIndex = 0;
	int mOldSelectIndex = 0;

	Font mNameFont;

public:
	void Initialize(const PlayerManager* plm);
	void Update();
	void Render(Vector2 leftBottom);
	void Release();

	bool IsChangedIndex()const { return mSelectIndex != mOldSelectIndex; }

	int GetIndex() const { return mSelectIndex; }
};