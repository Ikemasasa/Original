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

	// �S�s��1�s�������ɓ����
	void GetAllLine(std::vector<std::string>*outAllLine);

	// ���̍s�������ɓ����
	void GetNextLine(std::string * outLine);

	// �^�O�̎q�v�f���擾
	void GetTagNode(const std::string& tag, std::vector<std::string>* nodes, bool isCurrentPtr = false);

	// GetTagChild�Ŏ擾�����m�[�h���炳��Ƀ^�O���擾
	void GetTagNodeFromNode(const std::string& tag, const std::vector<std::string>& from, std::vector<std::string>* outNodes);

	// �v�f�𑮐���؂�ŕ���(0�Ԗڂ̓^�O��)
	void NodeSplitElement(const std::string& node, std::vector<std::string>* splitNode);

	// NodeSplitElement�Ŏ擾�����m�[�h���瑮�����A�����l���擾
	void GetElementValue(const std::string& splitNode, std::string* element, std::string* value);
};