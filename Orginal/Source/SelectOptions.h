#pragma once
#include <memory>
#include <string>

#include "lib/Font.h"
#include "lib/Vector.h"

class Sprite;

// 選択肢を選ぶクラス
class SelectOptions
{
	static constexpr float TITLE_STR_OFFSET_Y = 5.0f;
	static constexpr float OPTION_STR_OFFSET_X = 7.0f;
	static constexpr float SELECT_OFFSET_X = 5.0f;


	Font mFont;
	std::unique_ptr<Sprite> mOptionTitle;
	std::unique_ptr<Sprite> mOption;
	std::unique_ptr<Sprite> mOptionEnd;
	std::unique_ptr<Sprite> mOptionSelect;
	std::wstring mStrTitle;
	std::vector<std::wstring> mStrOptions;
	int mOptionIndex = 0;

public:
	SelectOptions() = default;
	~SelectOptions() = default;

	void Initialize();
	void Update();
	void Render(const Vector2& leftTop, bool isStrsClear = true);
	void AddTitle(const wchar_t* str);
	void AddOption(const wchar_t* str);

	int GetIndex() const { return mOptionIndex; }
};