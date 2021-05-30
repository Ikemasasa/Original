#pragma once

class PlayerManager;

class MenuBase
{
	friend class MenuManager;

protected:
	enum Select
	{
		ITEM,
		STATUS,

		SELECT_NUM,
		NONE = 10 // アイテム(ステータスでもいいけど)を一番上にするためにNONEを一番最後にしてる
	};

	int mSelectIndex = 0;

public:
	MenuBase() = default;
	virtual ~MenuBase() = default;

	virtual Select Update(PlayerManager* plm) = 0;
	virtual void Render() = 0;
};