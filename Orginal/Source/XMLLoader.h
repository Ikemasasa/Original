#pragma once
#include <fstream>
#include <string>
#include <vector>

class XMLLoader
{
	std::ifstream mFile;
	
	void EraseHeadSpace(std::string& line);
public:
	XMLLoader() = default;
	~XMLLoader() = default;

	bool Open(const char* xml);
	void Close();

	bool IsEndFile();

	// 全行を1行ずつ引数に入れる
	void GetAllLine(std::vector<std::string>*outAllLine);

	// 次の行を引数に入れる
	void GetNextLine(std::string * outLine);

	// タグの子要素を取得
	void GetTagNode(const std::string& tag, std::vector<std::string>* nodes, bool isCurrentPtr = false);

	// GetTagChildで取得したノードからさらにタグを取得
	void GetTagNodeFromNode(const std::string& tag, const std::vector<std::string>& from, std::vector<std::string>* outNodes);

	// 要素を属性区切りで分割(0番目はタグ名)
	void NodeSplitElement(const std::string& node, std::vector<std::string>* splitNode);

	// NodeSplitElementで取得したノードから属性名、属性値を取得
	void GetElementValue(const std::string& splitNode, std::string* element, std::string* value);
};