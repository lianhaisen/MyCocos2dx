#include "UIBuildingFactory.h"
#include "BuildingData.h"
#include "UICommon.h"
#include "BattleLogic.h"
#include "BuildingConfig.h"
#include "UILaserCannon.h"
#include "UICollector.h"
#include "UIContainer.h"
#include "UILightingTower.h"
#include "UISoldierCreator.h"
#include "UIBlockingTower.h"
#include "UISatelliteTower.h"
#include "UIPiercingCannon.h"
#include "UIBunkers.h"
#include "UIMulrocket.h"
#include "UIMortar.h"
#include "UIWall.h"
#include "UIStone.h"


#define  BaseBuildingId 740000

Building * UIBuildingFactory:: CreateBuildingFromDB(DBuilding * db)
{     
	 // 更新建筑物状态
	 db->getLeftTime();
     auto code = db->type / 100;
	 Building * result = nullptr;
     result = CreateBuildingFromType(code);
	 result->initWithDB(db);
	 return result;
}

Building * UIBuildingFactory::CreateBuildingFromBuyDB( BuyListConfig * db )
{
	auto code = db->buildcfg->id / 100;
	Building * result = nullptr;
    result = CreateBuildingFromType(code);
	result->initWithBuyDB(db);
	return result;
}

Building * UIBuildingFactory::CreateBuildingFromType( int type )
{   
	Building * result = nullptr;
	switch (type)
	{
	//阻击塔
	case  710001:
		  result = UIBlockingTower::create();
		  break;
	//电塔
	case  710005:
		  result = UILightingTower::create();
		  break;
	//卫星塔
	case  710011:
		 result = UISatelliteTower::create();
		 break;
	//穿甲炮
	case  710004:
	//防空炮塔
	case  710007:
	//巡航导弹
    case  710008:
		result = UIPiercingCannon::create();
		break;
	//碉堡
	case  710002:
		result = UIBunkers::create();
		break;
	//多发火箭炮
	case  710006:
		result = UIMulrocket::create();
		break;
	//迫击炮
	case  710003:
		result = UIMortar::create();
		break;
	//激光炮
	case  710009:
	case  710010:
		result = UILaserCannon::create();
		break;
		//采集器
	case 740001:
	case 740003:
	case 740005:
		result = UICollector::create();
		break;
		//仓库
	case 740002:
	case 740004:
	case 740006:
		result = UIContainer::create();
		break;

	case 740012:
	case 740013:
	case 740014:
		result = UISoldierCreator::create();
		break;
    
  	//能量墙
	case 720001:
		result = UIWall::create();
		break;
    //电网
	case 720002:
		break;
	//石台
	case 720003:
		result = UIStone::create();
		break;

	default:
		break;
	}
	if (result == nullptr)
	{
		result = UICommon::create();
	}
	return result;
}

Building* UIBuildingFactory::CreateBuildingFromBattleLogic( DBuilding *db )
{

	auto code = db->type / 100;
	Building * result = nullptr;
	result = CreateBuildingFromType(code);
	result->initWithBattleLogic(db);
	return result;
}
