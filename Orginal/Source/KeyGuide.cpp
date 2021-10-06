#include "KeyGuide.h"

KeyGuide::KeyGuide()
{
	mKeyIcons = std::make_unique<Sprite>(L"Data/Image/keyguide/key_icons.png");
	mKeyGuideBoard = std::make_unique<Sprite>(L"Data/Image/keyguide/keyguide_board.png");
	mFont.Initialize();
}

void KeyGuide::Add(Key* key, int keyNum, std::wstring description)
{
	std::vector<Key> keys;
	for (int i = 0; i < keyNum; ++i)
	{
		keys.emplace_back(key[i]);
	}

	Guide guide;
	guide.keys = keys;
	guide.description = L":" + description;
	mKeyGuides.emplace_back(guide);
}

void KeyGuide::Render(bool isClear)
{
	if (mKeyGuides.empty()) return;

	float totalX = 0.0f;
	for (auto& guide : mKeyGuides)
	{
		totalX += mFont.GetWidth(guide.description.c_str()) + guide.keys.size()* ICON_SCALE_SIZE;
	}

	// ボード描画
	mKeyGuideBoard->Render(Vector2(Define::SCREEN_WIDTH - totalX, BOARD_LEFTTOP_Y), Vector2::ONE, Vector2::ZERO, mKeyGuideBoard->GetSize());


	// フォントセット
	float offsetX = GUIDE_LEFTTOP_X;
	for (size_t i = 0; i < mKeyGuides.size(); ++i)
	{
		auto& guide = mKeyGuides[i];

		int keyNum = (int)guide.keys.size();
		float totalKeySize = keyNum * ICON_SCALE_SIZE;

		// キー描画
		for (size_t k = 0; k < guide.keys.size(); ++k)
		{
			offsetX += k * ICON_SCALE_SIZE;

			Vector2 pos(offsetX, GUIDE_LEFTTOP_Y);
			Vector2 scale(ICON_SCALE, ICON_SCALE);
			Vector2 tex(guide.keys[k] * ICON_SIZE, 0.0f);
			Vector2 size(ICON_SIZE, ICON_SIZE);
			mKeyIcons->Render(pos, scale, tex, size);
		}
		offsetX += ICON_SCALE_SIZE;

		// フォントセット
		mFont.RenderSet(guide.description.c_str(), Vector2(offsetX, GUIDE_LEFTTOP_Y));
		offsetX += mFont.GetWidth(guide.description.c_str()) + totalKeySize;
	}

	// フォント描画
	mFont.Render();


	if (isClear) mKeyGuides.clear();
}
