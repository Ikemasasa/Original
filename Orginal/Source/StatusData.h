#pragma once
#include <string>
#include <vector>

struct Status
{
	std::wstring name; // ���O
	int id;			  // ID
	int hp;			  // HP
	int mp;			  // MP
	int str;		  // ��b�U����
	int vit;		  // ��b�h���
	int agi;		  // �f����
};

class StatusData
{
	std::vector<Status> mPLStatus;
	std::vector<Status> mEnmStatus;

	void LoadPLStatus();
	void LoadEnmStatus();

public:
	StatusData();
	~StatusData();

	void Initialize();
	Status GetPLStatus(size_t id) const;
	Status GetEnmStatus(size_t id) const;

	void SetPLStatus(size_t charaID, const Status& status);
};