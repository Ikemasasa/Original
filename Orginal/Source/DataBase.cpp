#include "DataBase.h"

#include "ItemData.h"
#include "StatusData.h"

DataBase::DataBase()
{
	mStatusData = std::make_unique<StatusData>();
	mItemData   = std::make_unique<ItemData>();
}

DataBase::~DataBase()
{

}
