#pragma once
#include <list>
#include <memory>

#include "lib/Font.h"
#include "lib/Vector.h"

class Sprite;

class DropItemShower
{
	static constexpr float VELOCITY_Y = 1.5f;
	static constexpr float ALPHA_ADD = 0.05f;
	static constexpr float ALPHA_TURN_VALUE = 1.5f;

	static constexpr float BOARD_SCALE = 0.5f;
	static constexpr float STR_OFFSET_Y = 4.0f;

	struct Data
	{
		int itemID;
		Vector2 pos;
		float alpha;
		bool alphaTurn;
	};

	std::list<Data> mDropItemDatas;
	std::unique_ptr<Sprite> mBoard;
	Font mFont;

public:
	DropItemShower();
	~DropItemShower() = default;

	void Add(const int dropItemID, const Vector3& dropCharaPos);

	void Update();
	void Render();
};