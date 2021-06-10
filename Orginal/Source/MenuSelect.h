#pragma once
#include "MenuBase.h"

#include <memory>
#include <vector>
#include <string>

#include "lib/Vector.h"
#include "lib/Font.h"

class Sprite;

class MenuSelect : public MenuBase
{
private:
	static const int STR_MAX = 8;
	static const int STR_OFFSET_X = 10;
	static const int STR_OFFSET_Y = 0;

	static constexpr float PLATE_OFFSET_X = 100.0f;
	static constexpr float PLATE_OFFSET_Y = 50.0f;
	static constexpr float PLATE_FIRST_OFFSET_Y = 75.0f;

	static constexpr float FADE = 0.15f;

	const float MOVE_MAX = -30.0f; // data.pos + MOVE_MAX ÇÃà íuÇ™ç≈èâÇ…à íuÇ…Ç»ÇÈ


	struct Data
	{
		Sprite* plate;
		wchar_t str[STR_MAX];
		Vector2 pos;
		float moveX;
	};

	std::unique_ptr<Sprite> mSelectBar;
	std::unique_ptr<Sprite> mBar;
	std::vector<Data> mDatas;
	Font mFont;
	int mSelectIndex = 0;
	
	float mBeginMoveX = 0.0f;


	bool Add(Sprite* plate, const wchar_t* str);
public:
	MenuSelect();
	~MenuSelect() = default;

	void BeginAnimation();
	void Initialize(const PlayerManager* plm) override;
	Select Update(const PlayerManager* plm) override;
	void Render() override;
	void Release() override;
};