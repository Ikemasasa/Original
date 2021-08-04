#include "DataBase.h"

#include "ItemData.h"
#include "StatusData.h"
#include "EquipmentData.h"

DataBase::DataBase()
{
	mStatusData = std::make_unique<StatusData>();
	mItemData   = std::make_unique<ItemData>();
	mEquipmentData   = std::make_unique<EquipmentData>();
}

DataBase::~DataBase()
{

}
