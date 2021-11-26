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

	bool IsEndFile();

	// �S�s��1�s�������ɓ����
	void GetAllLine(std::vector<std::string>* outAllLine);

	// ���̍s�������ɓ����
	void GetNextLine(std::string* outLine);

	// 1�s����R���}��؂�ŕ���
	void GetChunks(const std::string& line, std::vector<std::string>* chunks, const char ignore = '#');
};