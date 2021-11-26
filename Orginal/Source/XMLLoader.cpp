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
		 // 1�s��(xml�̃o�[�W�����A�����t�H�[�}�b�g�Ƃ��������Ă�)�𖳎�����
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
		// eof�t���O�𖳎�
		mFile.clear();

		// �t�@�C���|�C���^��擪�Ɉړ�
		mFile.seekg(0);
	}

	nodes->clear();

	//���[�v�J�n
	bool find = false;
	std::string line;
	std::string tagName = '<' + tag + '>';
	while (std::getline(mFile, line))
	{
		// �擪�ɋ󔒂�����΍폜
		EraseHeadSpace(line);

		// �^�O�Ɠ������`�F�b�N
		if (line == tagName)
		{
			find = true;
			break;
		}
	}

	// �^�O�𔭌�������^�O�I����������܂�node�ɓ����
	std::string tagEnd = "</" + tag + '>';
	if (find)
	{
		while (std::getline(mFile, line))
		{
			// �擪�ɋ󔒂�����΍폜
			EraseHeadSpace(line);

			// �^�O�I���Ɠ������`�F�b�N
			if (line == tagEnd)
			{
				break;
			}

			// node�ɒǉ�
			nodes->emplace_back(line);
		}
	}
}

void XMLLoader::GetTagNodeFromNode(const std::string& tag, const std::vector<std::string>& nodes, std::vector<std::string>* outNodes)
{
	//���[�v�J�n
	bool find = false;
	std::string tagName = '<' + tag + '>';
	int i = 0;
	for (i = 0; i < nodes.size(); ++i)
	{
		// �^�O�����邩�`�F�b�N
		size_t pos = nodes[i].find(tagName);

		// �^�O������������
		if (pos != std::string::npos)
		{
			// �����t���Oon
			find = true;

			// �^�O�����������Ďc���outNodes�ɒǉ�
			std::string add = nodes[i];
			add.erase(pos, pos + tagName.size());
			if (add.size() > 0) outNodes->emplace_back(add);
			break;
		}
	}

	// �^�O�𔭌�������^�O�I����������܂�node�ɓ����
	if (find)
	{
		std::string tagEnd = "</" + tag + '>';
		
		// ��̃��[�v��i�����������g��
		++i; // �O��̍s����
		for (;i < nodes.size(); ++i)
		{
			// �I���^�O�����邩�`�F�b�N
			size_t pos = nodes[i].find(tagEnd);

			// �I���^�O������������
			if (pos != std::string::npos)
			{
				// �I���^�O�����������Ďc���outNodes�ɒǉ�
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
	// element="value" ���Ă����\����z��
	element->clear();
	value->clear();


	for (int i = 0; i < splitNode.size(); ++i)
	{
		// �C�R�[�����o��܂�element�ɒǉ�
		if (splitNode[i] != '=')
		{
			*element += splitNode[i];
		}
		else
		{
			// �C�R�[�����o���瑮���l���l����

			i++; // �C�R�[���𖳎�

			bool dqFind = false; // �_�u���N�H�[�e�[�V����find
			while (splitNode[i])
			{
				// �_�u���N�H�[�e�[�V�������������瑮���l�ɒǉ����Ă���
				if (dqFind)
				{
					// �܂��_�u���N�H�[�e�[�V��������������I��
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