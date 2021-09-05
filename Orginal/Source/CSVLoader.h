#pragma once
#include <fstream>
#include <string>
#include <vector>

class CSVLoader
{
	std::ifstream mFile;

public:
	CSVLoader() = default;
	~CSVLoader() = default;

	bool Open(const char* csv);
	void Close();

	// 全行を1行ずつ引数に入れる
	void GetAllLine(std::vector<std::string>* outAllLine);

	// 1行からコンマ区切りで分割
	void GetChunks(const std::string& line, std::vector<std::string>* chunks, const char ignore = '#');
};