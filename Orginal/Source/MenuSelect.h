#pragma once
#include <memory>
#include <vector>
#include <string>

#include "lib/Vector.h"
#include "lib/Font.h"

class Texture;

class MenuSelect
{
public:
	enum Select
	{
		SELECT_ITEM,
		SELECT_STATUS,

		SELECT_MENU = 10  // 10は適当、最後の要素にしたかっただけ
	};

private:
	static const int STR_MAX = 8;
	static const int STR_OFFSET_X = 40;
	static const int STR_OFFSET_Y = 0;

	static constexpr float BAR_OFFSET_X = 100.0f;
	static constexpr float BAR_OFFSET_Y = 50.0f;
	static constexpr float BAR_FIRST_OFFSET_Y = 75.0f;

	static const int SELECT_NUM = 2;
	static constexpr float SELECT_OFFSET_X = -50.0f;

	static constexpr float FADE = 0.15f;

	const float MOVE_MAX = -30.0f; // data.pos + MOVE_MAX の位置が最初に位置になる


	struct Data
	{
		Texture* bar;
		wchar_t str[STR_MAX];
		Vector2 pos;
		float moveX;
	};

	std::unique_ptr<Texture> mSelectBar;
	std::unique_ptr<Texture> mBar;
	std::vector<Data> mDatas;
	std::vector<std::wstring> mSelectStrs;
	Font mFont;
	int mSelectIndex = 0;
	
	float mBeginMoveX = 0.0f;


	bool Add(Texture* bar, const wchar_t* str);
public:
	MenuSelect();
	~MenuSelect() = default;

	void BeginAnimation();
	Select Update();
	void Render();
	void Release();
};