#pragma once
#include "SceneBase.h"

#include "lib/Font.h"
#include "lib/Sprite.h"

class SceneTitle : public SceneBase
{
	static const int FONT_LOGO_SIZE = 128;
	static const int FONT_LOGO_WEIGHT = 64;

	static constexpr float STR_OFFSET = 5;

	enum SelectCmd
	{
		GAME_START,
		EXIT,
		MAX,
	};

	Font mFont;
	Font mFontLogo;
	std::unique_ptr<Sprite> mBG;
	std::unique_ptr<Sprite> mBoard;
	std::unique_ptr<Sprite> mBoardSelect;

	int mSelectIndex = 0;
	bool mIsPressAButton = false;
	float mSinAngle = 0.0f;

public:
	SceneTitle();
	~SceneTitle();

	void Initialize();
	void Update();
	void Render();
	void Release();
};

