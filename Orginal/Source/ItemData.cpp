#include "ItemData.h"

#include <string>

#include "lib/ConvertString.h"
#include "lib/Sprite.h"

#include "CSVLoader.h"
#include "PlayerManager.h"

std::vector<ItemData::BaseData> ItemData::mItems;

void ItemData::Initialize()
{
	LoadItemData();
}

void ItemData::Release()
{
	mItems.clear();
}

const ItemData::BaseData* ItemData::GetBaseData(const size_t id)
{
	ItemData::BaseData* ret = nullptr;

	for (auto& basedata : mItems)
	{
		if (basedata.id == id)
		{
			ret = &basedata;
			break;
		}
	}

	return ret;
}

void ItemData::LoadItemData()
{
	const char* filename = "Data/Database/ItemBase.csv";
	std::string iconPath = "Data/Image/ItemIcon/";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	for (const auto& line : allLine)
	{
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		if (chunks.size() > 0)
		{
			int index = 0;
			BaseData base;
			base.name = ConvertString::ConvertToWstring(chunks[index++]);
			base.id = std::stoi(chunks[index++]);
			base.type = (ItemType)std::stoi(chunks[index++]);
			base.icon = std::make_shared<Sprite>(ConvertString::ConvertToWstring(iconPath + chunks[index++]).c_str());
			base.info = ConvertString::ConvertToWstring(chunks[index++]);

			mItems.emplace_back(base);
		}
	}

	loader.Close();
}

//--------------------------------------------------------------
// UseItemData
//--------------------------------------------------------------
const UseItemData::Param UseItemData::GetParam(const size_t id)
{
	// ベースデータ取得
	const ItemData::BaseData* base = ItemData::GetBaseData(id);

	const char* filename = nullptr;
	if (base->type == ItemData::HEAL)   filename = "Data/DataBase/HealItemData.csv"; 
	if (base->type == ItemData::DAMAGE) filename = "Data/DataBase/DamageItemData.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);


	// 目的の行を取得
	int lineIndex = 0;
	if (base->type == ItemData::HEAL)   lineIndex = id - DataBase::HEAL_ITEM_ID_START;
	if (base->type == ItemData::DAMAGE) lineIndex = id - DataBase::DAMAGE_ITEM_ID_START;
	std::vector<std::string> chunks;
	loader.GetChunks(allLine[lineIndex], &chunks);
	
	Param ret = {};
	if (chunks.size() > 0)
	{
		int index = 2; // アイテム名、アイテムIDを無視する
		ret.base = base;
		ret.target = (Target)std::stoi(chunks[index++]);
		ret.range = (Range)std::stoi(chunks[index++]);
		ret.rate = (Rate)std::stoi(chunks[index++]);
		ret.hpValue = std::stoi(chunks[index++]);
		ret.mpValue = std::stoi(chunks[index++]);
	}

	return ret;
}

//--------------------------------------------------------------
// EquipmentData
//--------------------------------------------------------------
const FieldUseItemData::Param FieldUseItemData::GetParam(const size_t id)
{
	Param param;
	param.base = ItemData::GetBaseData(id);
	switch (id)
	{
	case ItemData::SUBDUE_PROOF: param.effect = std::make_shared<SubDueProofEffect>(); break;
	}

	return param;
}

//--------------------------------------------------------------
// EquipmentData
//--------------------------------------------------------------
const EquipmentData::Param EquipmentData::GetParam(const size_t id)
{
	const char* filename = "Data/DataBase/EquipmentData.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	// 目的の行を取得
	const int LINE_INDEX = id - DataBase::EQUIPMENT_ID_START;
	std::vector<std::string> chunks;
	loader.GetChunks(allLine[LINE_INDEX], &chunks);

	Param ret = {};
	if (chunks.size() > 0)
	{
		int index = 2; // アイテム名、アイテムIDを無視する
		ret.base = ItemData::GetBaseData(id);
		ret.type	 = (Type)std::stoi(chunks[index++]);
		ret.atk		 = std::stoi(chunks[index++]);
		ret.def		 = std::stoi(chunks[index++]);
		ret.spd		 = std::stoi(chunks[index++]);
		
		for (size_t i = index; i < chunks.size(); ++i)
		{
			bool equipable = false;
			if (chunks[i] == "o") equipable = true;
			ret.equipable.push_back(equipable);
		}
	}

	return ret;
}

