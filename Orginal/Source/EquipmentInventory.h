#pragma once
#include "ItemData.h"

class Character;

/*
	装備品のインベントリクラス
	消費アイテムとかのインベントリと分けたかった
*/
class EquipmentInventory
{
public: // 変数
	struct Data
	{
		const Character* equipChara = nullptr; // 装備しているキャラ
		int equipmentID = -1;				   // 装備品のID
	};

private:
	std::vector<Data> mEquipmentsData[EquipmentData::TYPE_NUM]; // タイプごとの装備品のリスト

public: // 関数

	// コンストラクタ
	EquipmentInventory();

	// デストラクタ
	~EquipmentInventory();

	// インベントリに追加
	void Push(int id);

	// インベントリから削除
	void Remove(int id);

	// インベントリを取得
	const std::vector<Data>& GetInventory(EquipmentData::Type type) const { return mEquipmentsData[type]; }

	// 装備キャラに登録
	void SetChara(EquipmentData::Type type, const Character* chara, int inventoryIndex) { mEquipmentsData[type][inventoryIndex].equipChara = chara; }

	// 装備キャラから抹消
	void UnSetChara(EquipmentData::Type type, const Character* chara);
};