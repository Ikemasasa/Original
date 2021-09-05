#pragma once
#include "MenuBase.h"

#include <memory>
#include <vector>
#include <string>

#include "lib/Vector.h"
#include "lib/Font.h"

class Sprite;
class CharacterHealth;

class MenuSelect : public MenuBase
{
private:
	static constexpr float BOARD_POS_X = 100.0f;
	static constexpr float BOARD_POS_Y = 75.0f;
	static constexpr float BOARD_OFFSET_Y = 15.0f;

	static constexpr float HEALTH_BOARD_POS_X = 800.0f;

	static const int STR_OFFSET_X = 5;
	static const int STR_OFFSET_Y = 5;

	static constexpr float FADE = 0.15f;

	const float MOVE_MAX = 30.0f; // data.pos + MOVE_MAX ÇÃà íuÇ™ç≈èâÇ…à íuÇ…Ç»ÇÈ


	struct Data
	{
		Vector2 pos;
		float moveX;
	};

	std::unique_ptr<Sprite> mStrBoard;
	std::unique_ptr<Sprite> mStrSelect;
	std::unique_ptr<CharacterHealth> mCharacterHealth;
	std::vector<Data> mDatas;
	Font mFont;

public:
	MenuSelect() = default;
	~MenuSelect() = default;

	void BeginAnimation();
	void Initialize(const PlayerManager* plm) override;
	Select Update(PlayerManager* plm) override;
	void Render() override;
	void Release() override;
};