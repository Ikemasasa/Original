#include "EquipmentData.h"

#include <fstream>
#include <sstream>

#include "lib/ConvertString.h"
#include "lib/Sprite.h"

#include "PlayerManager.h"

EquipmentData::EquipmentData()
{
	mEquipments.clear();
	Initialize();
}

EquipmentData::~EquipmentData()
{
	mEquipments.clear();
}

void EquipmentData::Initialize()
{
	const char* filename = "Data/DataBase/EquipmentData.csv";

	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) return;

	std::string line;
	const char delim = ',';
	const char* iconPath = "Data/Image/ItemIcon/";
	while (std::getline(fin, line))
	{
		std::istringstream istr(line);
		std::string chunk;

		std::vector<std::string> data;
		while (std::getline(istr, chunk, delim))
		{
			if (chunk.empty()) continue;
			if (chunk[0] == '#') continue;

			data.emplace_back(chunk);
		}

		if (data.size() > 0)
		{
			Param param;
			int index = 0;

			param.name = ConvertString::ConvertToWstirng(data[index++]);
			param.id = std::stoi(data[index++]);
			param.icon = std::make_shared<Sprite>(ConvertString::ConvertToWstirng(iconPath + data[index++]).c_str());
			param.type = (Type)std::stoi(data[index++]);
			param.atk = std::stoi(data[index++]);
			param.def = std::stoi(data[index++]);
			param.spd = std::stoi(data[index++]);
			
			for (int i = 0; i < PlayerManager::ALL_PLAYER_NUM; ++i)
			{
				bool equipable = false;
				if (strcmp("o", data[index++].c_str()) == 0)
				{
					equipable = true;
				}
				param.equipable.push_back(equipable);
			}

			mEquipments.push_back(param);
		}
	}
}

const EquipmentData::Param* EquipmentData::GetParam(size_t id) 
{
	return &mEquipments[id - DataBase::EQUIPMENT_ID_START];
}
