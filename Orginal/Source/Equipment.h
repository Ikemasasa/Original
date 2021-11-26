#pragma once
#include <vector>

#include "ItemData.h"

class Character;

/*
	装備クラス
	Statusクラスに内臓されてる
*/
class Equipment
{
	// 変数
	std::vector<EquipmentData::Param> mEquipments; // 装備している物リスト

public: // 関数

	// 装備
	bool Equip(const Character* equipChara, int id);

	// 装備を外す
	void UnEquip(int id);
	void UnEquip(EquipmentData::Type type);

	// 装備している物のパラメータを取得
	const EquipmentData::Param* GetParam(const EquipmentData::Type type) const;

	// 装備しているものの数を取得
	size_t GetNum() const { return mEquipments.size(); }
};