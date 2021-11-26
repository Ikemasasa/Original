#include "XMLLoader.h"

#include <sstream>

#include <locale>
#include <iostream>
#include <string>


bool XMLLoader::Open(const char* xml)
{
	 

	 mFile.open(xml);

	 bool isOpen = mFile.is_open();
	 if (isOpen)
	 {
		 // 1行目(xmlのバージョン、文字フォーマットとかが書いてる)を無視する
		 std::string ignore;
		 GetNextLine(&ignore);
	 }

	 return isOpen;
}

void XMLLoader::Close()
{
	mFile.close();
}

bool XMLLoader::IsEndFile()
{
	return mFile.eof();
}

void XMLLoader::GetAllLine(std::vector<std::string>* outAllLine)
{
	outAllLine->clear();

	std::string line;
	while (std::getline(mFile, line))
	{
		outAllLine->emplace_back(line);
	}
}

void XMLLoader::GetNextLine(std::string* outLine)
{
	std::getline(mFile, *outLine);
}

void XMLLoader::GetTagNode(const std::string& tag, std::vector<std::string>* nodes, bool isCurrentPtr)
{
	if (!mFile) return;

	if (!isCurrentPtr)
	{
		// eofフラグを無視
		mFile.clear();

		// ファイルポインタを先頭に移動
		mFile.seekg(0);
	}

	nodes->clear();

	//ループ開始
	bool find = false;
	std::string line;
	std::string tagName = '<' + tag + '>';
	while (std::getline(mFile, line))
	{
		// 先頭に空白があれば削除
		EraseHeadSpace(line);

		// タグと同じかチェック
		if (line == tagName)
		{
			find = true;
			break;
		}
	}

	// タグを発見したらタグ終了を見つけるまでnodeに入れる
	std::string tagEnd = "</" + tag + '>';
	if (find)
	{
		while (std::getline(mFile, line))
		{
			// 先頭に空白があれば削除
			EraseHeadSpace(line);

			// タグ終了と同じかチェック
			if (line == tagEnd)
			{
				break;
			}

			// nodeに追加
			nodes->emplace_back(line);
		}
	}
}

void XMLLoader::GetTagNodeFromNode(const std::string& tag, const std::vector<std::string>& nodes, std::vector<std::string>* outNodes)
{
	//ループ開始
	bool find = false;
	std::string tagName = '<' + tag + '>';
	int i = 0;
	for (i = 0; i < nodes.size(); ++i)
	{
		// タグがあるかチェック
		size_t pos = nodes[i].find(tagName);

		// タグが見つかったら
		if (pos != std::string::npos)
		{
			// 発見フラグon
			find = true;

			// タグ部分を消して残りをoutNodesに追加
			std::string add = nodes[i];
			add.erase(pos, pos + tagName.size());
			if (add.size() > 0) outNodes->emplace_back(add);
			break;
		}
	}

	// タグを発見したらタグ終了を見つけるまでnodeに入れる
	if (find)
	{
		std::string tagEnd = "</" + tag + '>';
		
		// 上のループのiを引き続き使う
		++i; // 前回の行無視
		for (;i < nodes.size(); ++i)
		{
			// 終了タグがあるかチェック
			size_t pos = nodes[i].find(tagEnd);

			// 終了タグが見つかったら
			if (pos != std::string::npos)
			{
				// 終了タグ部分を消して残りをoutNodesに追加
				std::string add = nodes[i];
				add.erase(pos, pos + tagName.size());
				if (add.size() > 0) outNodes->emplace_back(add);
				break;
			}

			outNodes->emplace_back(nodes[i]);
		}
	}
}

void XMLLoader::NodeSplitElement(const std::string& node, std::vector<std::string>* splitNode)
{
	splitNode->clear();

	std::istringstream istr(node);
	std::string chunk;
	static const char DELIM = ' ';
	while (std::getline(istr, chunk, DELIM))
	{
		if (chunk[0] == '<') chunk.erase(chunk.begin());
		if (chunk == "/>") chunk.clear();
		if (chunk.empty()) continue;

		splitNode->emplace_back(chunk);
	}
}

void XMLLoader::GetElementValue(const std::string& splitNode, std::string* element, std::string* value)
{
	// element="value" っていう構造を想定
	element->clear();
	value->clear();


	for (int i = 0; i < splitNode.size(); ++i)
	{
		// イコールが出るまでelementに追加
		if (splitNode[i] != '=')
		{
			*element += splitNode[i];
		}
		else
		{
			// イコールが出たら属性値を考える

			i++; // イコールを無視

			bool dqFind = false; // ダブルクォーテーションfind
			while (splitNode[i])
			{
				// ダブルクォーテーションを見つけたら属性値に追加していく
				if (dqFind)
				{
					// またダブルクォーテーションを見つけたら終了
					if (splitNode[i] == '\"') break;
					*value += splitNode[i];
				}

				if (splitNode[i] == '\"')
				{
					dqFind = true;
				}

				++i;
			}
			break;
		}
	}


}


void XMLLoader::EraseHeadSpace(std::string& line)
{
	char spaceCount = 0;
	for (int i = 0; i < line.size(); ++i)
	{
		if (line[i] == ' ') ++spaceCount;
		else
		{
			if (spaceCount > 0) line.erase(0, spaceCount);
			break;
		}
	}
}