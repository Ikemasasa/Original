#include "ItemData.h"

#include <fstream>
#include <string>
#include <sstream>

#include "lib/ConvertString.h"
#include "lib/Texture.h"

ItemData::ItemData()
{
	mItems.clear();

	LoadItemData();
}

ItemData::~ItemData()
{
	mItems.clear();
}

void ItemData::LoadItemData()
{
	const char* filename = "Data/Database/ItemData.csv";
	std::string iconPath = "Data/Image/ItemIcon/";

	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) return;

	std::string line;  // 1�s�擾�p
	const char delim = ','; // ��؂蕶��
	while (std::getline(fin, line)) // ��s�ǂݍ���
	{
		std::istringstream istr(line);
		std::string chunk; // �R���}��؂�̓��e�擾�p

		std::vector<std::string> data;

		while (std::getline(istr, chunk, delim))
		{
			if (chunk.empty()) continue;   // �󔒂Ȃ� continue
			if (chunk[0] == '#') continue; // �ŏ��̕�����#�Ȃ�continue

			data.emplace_back(chunk);
		}

		if (data.size() > 0)
		{
			ItemParam item = {};
			int index = 0;
			item.id = std::stoi(data[index++]);
			item.name = data[index++];
			item.icon = std::make_shared<Texture>(ConvertString::ConvertToWstirng(iconPath + data[index++]).c_str());
			item.effect = (Effect)std::stoi(data[index++]);
			item.target = (Target)std::stoi(data[index++]);
			item.range = (Range)std::stoi(data[index++]);
			item.rate = (Rate)std::stoi(data[index++]);
			item.hpValue = std::stoi(data[index++]);
			item.mpValue = std::stoi(data[index++]);
			item.atkValue = std::stoi(data[index++]);
			item.defValue = std::stoi(data[index++]);
			item.info = data[index++];

			mItems.emplace_back(item);
		}
	}

	fin.close();
}
