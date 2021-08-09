#pragma once

class PlayerManager;

class MenuBase
{
	friend class MenuManager;

protected:
	enum Select
	{
		ITEM,
		EQUIPMENTS,
		STATUS,
		SELECT_NUM,

		BACK,
		NONE = 10 // アイテム(ステータスでもいいけど)を一番上にするためにNONEを一番最後にしてる
	};

	int mSelectIndex = 0;

public:
	MenuBase() = default;
	virtual ~MenuBase() = default;

	virtual void Initialize(const PlayerManager* plm) = 0;
	virtual Select Update(PlayerManager* plm) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};