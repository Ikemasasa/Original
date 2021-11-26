#include "DataBase.h"

#include "DropData.h"
#include "ItemData.h"
#include "StatusData.h"
#include "SkillData.h"

void DataBase::Initialize()
{
	// �e��f�[�^�̏������֐����Ă�
	ItemData::Initialize();
	StatusData::Initialize();
	SkillData::Initialize();
}

void DataBase::Release()
{
	SkillData::Release();
	StatusData::Release();
	ItemData::Release();
}
