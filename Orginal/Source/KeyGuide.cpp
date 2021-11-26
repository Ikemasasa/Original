#include "KeyGuide.h"

#include <algorithm>

#include "lib/Math.h"

KeyGuide::KeyGuide()
{
	// 画像読み込み
	mKeyIcons = std::make_unique<Sprite>(L"Data/Image/keyguide/key_icons.png");
	mKeyGuideBoard = std::make_unique<Sprite>(L"Data/Image/keyguide/keyguide_board.png");

	// フォント初期化
	mFont.Initialize();
}

std::list<KeyGuide::Guide>::iterator KeyGuide::GetInsertIterator(Key key)
{
	// enum のkey順に並べるため(昇順
	auto it = mKeyGuides.begin();
	for (; it != mKeyGuides.end(); ++it)
	{
		if (it->keys[0] > key) break;
	}

	return it;
}

bool KeyGuide::ExistKey(Key key)
{
	for (const auto& guide : mKeyGuides)
	{
		if (guide.keys[0] == key)
		{
			return true;
		}
	}

	return false;
}

void KeyGuide::Add(Key key, std::wstring description)
{
	// すでにkeyがあったらreturn 
	if (ExistKey(key)) return;

	// 追加
	Guide guide;
	guide.keys.emplace_back(key);
	guide.description = L":" + description;
	mKeyGuides.insert(GetInsertIterator(guide.keys[0]), guide);
}

void KeyGuide::Add(Key* key, int keyNum, std::wstring description)
{
	// すでにkeyがあったらreturn 
	if (ExistKey(key[0])) return;

	// 追加
	Guide guide;
	for (int i = 0; i < keyNum; ++i)
	{
		guide.keys.emplace_back(key[i]);
	}
	std::sort(guide.keys.begin(), guide.keys.end());
	guide.description = L":" + description;

	
	mKeyGuides.insert(GetInsertIterator(guide.keys[0]), guide);
}


void KeyGuide::Render(bool isClear)
{
	if (mKeyGuides.empty()) return;

	float totalX = 0.0f;
	for (auto& guide : mKeyGuides)
	{
		totalX += mFont.GetWidth(guide.description.c_str()) + guide.keys.size() * ICON_SCALE_SIZE + ICON_SCALE_SIZE;
	}

	// ボード描画
	mKeyGuideBoard->Render(Vector2(mMoveX - GUIDE_LEFTTOP_X, BOARD_LEFTTOP_Y), Vector2::ONE, Vector2::ZERO, mKeyGuideBoard->GetSize());


	// フォントセット
	const float POS_Y = BOARD_LEFTTOP_Y + GUIDE_LEFTTOP_Y;
	float offsetX = mMoveX;
	for (auto& guide : mKeyGuides)
	{
		int keyNum = (int)guide.keys.size();
		float totalKeySize = keyNum * ICON_SCALE_SIZE;

		// キー描画
		for (size_t k = 0; k < guide.keys.size(); ++k)
		{

			Vector2 pos(offsetX, POS_Y);
			Vector2 scale(ICON_SCALE, ICON_SCALE);
			Vector2 tex(guide.keys[k] * ICON_SIZE, 0.0f);
			Vector2 size(ICON_SIZE, ICON_SIZE);
			mKeyIcons->Render(pos, scale, tex, size);

			offsetX += ICON_SCALE_SIZE;
		}

		// フォントセット
		mFont.RenderSet(guide.description.c_str(), Vector2(offsetX, POS_Y), Vector2::ZERO, Define::FONT_COLOR);
		offsetX += mFont.GetWidth(guide.description.c_str()) + ICON_SCALE_SIZE;
	}

	// フォント描画
	mFont.Render();

	// mMoveX更新
	mMoveX = Math::Lerp(mMoveX, Define::SCREEN_WIDTH - totalX , LERP_FACTOR);

	if (isClear) mKeyGuides.clear();
}
