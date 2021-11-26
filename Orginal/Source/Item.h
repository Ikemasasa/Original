#pragma once
#include <list>
#include <iterator>

#include "DataBase.h"
#include "ItemData.h"

/*
	消費アイテムのインベントリクラス
*/
class ItemInventory
{
	// 変数
	std::list<ItemData::BaseData> mItems; // 所持アイテムのリスト

public: // 関数

	// インベントリに追加
	void Add(const int itemID);

	// インベントリから削除
	void Sub(const int itemID);

	// インベントリのアイテムの数を取得
	int GetItemNum() const { return static_cast<int>(mItems.size()); } 

	// インベントリからアイテムのパラメータ取得
	const ItemData::BaseData* GetItemParam(const int index) const;
};