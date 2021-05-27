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

	std::string line;  // 1行取得用
	const char delim = ','; // 区切り文字
	while (std::getline(fin, line)) // 一行読み込み
	{
		std::istringstream istr(line);
		std::string chunk; // コンマ区切りの内容取得用

		std::vector<std::string> data;

		while (std::getline(istr, chunk, delim))
		{
			if (chunk.empty()) continue;   // 空白なら continue
			if (chunk[0] == '#') continue; // 最初の文字が#ならcontinue

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
