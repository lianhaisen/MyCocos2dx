
#ifndef __BUILDINGFACTORY_H__
#define __BUILDINGFACTORY_H__

class Building;
class DBuilding;
class BuyListConfig;
#include "MapHelper.h"

class UIBuildingFactory
{
public:
	UIBuildingFactory(){}
	~UIBuildingFactory(){}

	 static Building * CreateBuildingFromType(int type);
	 //正常数据库读取 
     static Building * CreateBuildingFromDB(DBuilding * db);
	 //新建读取
	 static Building * CreateBuildingFromBuyDB(BuyListConfig * db);

	 static Building* CreateBuildingFromBattleLogic(DBuilding *db);

private:

};


#endif



