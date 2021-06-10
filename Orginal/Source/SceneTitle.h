#pragma once
#include "SceneBase.h"

#include "lib/Font.h"
#include "lib/Sprite.h"

class SceneTitle : public SceneBase
{
	static const int LOGO_FONT_SIZE   = 128;
	static const int LOGO_FONT_WEIGHT = 128;

	static const int SELECT_FONT_SIZE = 64;
	static const int SELECT_FONT_WEIGHT = 64;

	enum SelectCmd
	{
		GAME_START,
		EXIT,
		MAX,
	};

	Font mFontLogo;
	Font mFontSelectStr;
	std::unique_ptr<Sprite> mBG;
	std::unique_ptr<Sprite> mArrow;

	int mSelectIndex = 0;
	bool mIsPressAButton = false;

public:
	SceneTitle();
	~SceneTitle();

	void Initialize();
	void Update();
	void Render();
	void Release();
};

