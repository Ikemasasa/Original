#include "DataBase.h"

#include "DropData.h"
#include "EquipmentData.h"
#include "ItemData.h"
#include "StatusData.h"

DataBase::DataBase()
{
	mStatusData	   = std::make_unique<StatusData>();
	mItemData	   = std::make_unique<ItemData>();
	mEquipmentData = std::make_unique<EquipmentData>();
	mDropData      = std::make_unique<DropData>();
}

DataBase::~DataBase()
{

}
