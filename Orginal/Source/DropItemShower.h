#pragma once
#include <list>
#include <memory>

#include "lib/Font.h"
#include "lib/Vector.h"

class Sprite;

class DropItemShower
{
	static constexpr float VELOCITY_Y = 0.5f;
	static constexpr float ALPHA_ADD = 0.1f;

	static constexpr float DISP_SEC = 5.0f;

	static constexpr float STR_OFFSET_Y = 5.0f;

	struct Data
	{
		int itemID;
		Vector2 pos;
		float alpha;
		float alphaAdd;
		float dispTimer;
		bool fade;
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