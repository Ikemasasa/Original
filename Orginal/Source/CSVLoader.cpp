#include "CSVLoader.h"

#include <sstream>

bool CSVLoader::Open(const char* csv)
{
	// Šù‚ÉŠJ‚¢‚Ä‚¢‚éê‡‚Í false
	if (mFile.is_open()) return false;
	
	mFile.open(csv);
	return mFile.is_open();
}

void CSVLoader::Close()
{
	mFile.close();
}

bool CSVLoader::IsEndFile()
{
	return mFile.eof();
}

void CSVLoader::GetAllLine(std::vector<std::string>* outAllLine)
{
	std::string line;
	while (std::getline(mFile, line))
	{
		outAllLine->emplace_back(line);
	}
}

void CSVLoader::GetNextLine(std::string* outLine)
{
	std::getline(mFile, *outLine);
}

void CSVLoader::GetChunks(const std::string& line, std::vector<std::string>* chunks, const char ignore)
{
	std::istringstream istr(line);
	std::string chunk;
	const char COMMA = ',';
	while (std::getline(istr, chunk, COMMA))
	{
		if (chunk.empty()) continue;
		if (chunk[0] == ignore) continue;

		chunks->emplace_back(chunk);
	}
}
