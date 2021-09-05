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
}

void DataBase::Release()
{
	StatusData::Release();
	ItemData::Release();
}
