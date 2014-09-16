
#include "BuildingCommon.h"
#include "UIBuildingFactory.h"
#include "UIBuildingBase.h"
#include "cocostudio/CocoStudio.h"
#include "AppMacros.h"
using namespace cocostudio;

std::string BuildingCommon::getFilePathFromId( int id )
{
	char str[500];
	sprintf(str,"building/%d.png",id);
	return std::string(str);
}

std::string BuildingCommon::getStringFromLevelAndStatus( int level,DBuilding::BuildingStatus status )
{
	char sz[50];
	switch (status)
	{
	case DBuilding::BuildingStatus::RELEX:
		sprintf(sz,"%02d_nor",level);
		break;
	case DBuilding::BuildingStatus::UPGRADE:
		sprintf(sz,"%02d_nor",level);
		break;
	case DBuilding::BuildingStatus::WORK:
		sprintf(sz,"%02d_nor",level);
		break;
	case DBuilding::BuildingStatus::NewBuild:
		sprintf(sz,"%02d_nor",level);
	default:
		break;
	}
	return std::string(sz);
}

std::string BuildingCommon::getNomalTowerStringFromLevel( int level,int degree /*= 1*/ )
{
	auto sz =String::createWithFormat("%02d_head%02d_nor",level,degree);
	return sz->getCString();
}

std::string BuildingCommon::getTowerStringFromLevel( int level,int degree /*= 1*/ )
{
	auto sz =String::createWithFormat("%02d_head%02d",level,degree);
	return sz->getCString();
}
std::string BuildingCommon::getTowerBodyStringFromLevel( int level )
{
	auto sz =String::createWithFormat("%02d_body",level);
	return sz->getCString();
}




void BuildingCommon::LoadArmateResource()
{
	auto adm = ArmatureDataManager::getInstance();
	adm->addArmatureFileInfo("building/building/building.ExportJson");
	adm->addArmatureFileInfo("building/wall/wall.ExportJson");
	//adm->addArmatureFileInfo("building/dizuo/dizuo.ExportJson");

	//adm->addArmatureFileInfo("building/710001/710001.ExportJson");
	//adm->addArmatureFileInfo("building/710002/710002.ExportJson");
	//adm->addArmatureFileInfo("building/710003/710003.ExportJson");
	//adm->addArmatureFileInfo("building/710004/710004.ExportJson");
	//adm->addArmatureFileInfo("building/710005/710005.ExportJson");
	//adm->addArmatureFileInfo("building/710006/710006.ExportJson");
	//adm->addArmatureFileInfo("building/710007/710007.ExportJson");
	//adm->addArmatureFileInfo("building/710008/710008.ExportJson");
	//adm->addArmatureFileInfo("building/710009/710009.ExportJson");
	//adm->addArmatureFileInfo("building/710010/710010.ExportJson");
	//adm->addArmatureFileInfo("building/710011/710011.ExportJson");
 //   

	//adm->addArmatureFileInfo("building/740001/740001.ExportJson");
	//adm->addArmatureFileInfo("building/740002/740002.ExportJson");
	//adm->addArmatureFileInfo("building/740003/740003.ExportJson");
	//adm->addArmatureFileInfo("building/740004/740004.ExportJson");
	//adm->addArmatureFileInfo("building/740005/740005.ExportJson");
	//adm->addArmatureFileInfo("building/740006/740006.ExportJson");
	//adm->addArmatureFileInfo("building/740007/740007.ExportJson");
	//adm->addArmatureFileInfo("building/740008/740008.ExportJson");
	//adm->addArmatureFileInfo("building/740009/740009.ExportJson");
	//adm->addArmatureFileInfo("building/740010/740010.ExportJson");
	//adm->addArmatureFileInfo("building/740011/740011.ExportJson");
	//adm->addArmatureFileInfo("building/740012/740012.ExportJson");
	//adm->addArmatureFileInfo("building/740013/740013.ExportJson");
	//adm->addArmatureFileInfo("building/740014/740014.ExportJson");
	//adm->addArmatureFileInfo("building/740015/740015.ExportJson");
	//adm->addArmatureFileInfo("building/740016/740016.ExportJson");
	//adm->addArmatureFileInfo("building/740017/740017.ExportJson");
	//adm->addArmatureFileInfo("building/740018/740018.ExportJson");
	//adm->addArmatureFileInfo("building/740019/740019.ExportJson");
	//adm->addArmatureFileInfo("building/740020/740020.ExportJson");

	
}

std::string BuildingCommon::getArmatureNameFromType( int type )
{  
	if (type >= 740001 && type <= 740020)
	{
		return String::createWithFormat("%d",type)->getCString();
	}

	if (type >= 710001 && type <= 710011)
	{
		return String::createWithFormat("%d",type)->getCString();
	}
	if (type == 720001)
	{
		return "wall";
	}
	if (type == 720003)
	{
		return "dizuo";
	}
	std::string result;
	return result;
}

UIPanZoomLayer * BuildingCommon::getPlayer( bool isInbattle ,const std::vector<DBuilding*>* db ,const ccMenuCallback& buildingselect)
{
	auto pzLayer = UIPanZoomLayer::create();
	pzLayer->setMap(getMainMap(),isInbattle);
	//pzLayer->setScale( 0.5f );
	if (db)
	{   
		auto sdb = *db;
		for (auto item : sdb)
		{   
			auto build = UIBuildingFactory::CreateBuildingFromDB(item);
			build->setClickTarget(buildingselect);
			pzLayer->addBuilding(build,(MAX_GRID_COUNT  * item->coord.y) +  item->coord.x);
		}
	}
	

	
	return pzLayer;
}


std::string BuildingCommon::getPicFromId( int id )
{
	auto sid = id / 100;
	auto str = String::createWithFormat("building/%d.png",sid);
	auto img = Director::getInstance()->getTextureCache()->addImage(str->getCString());
	if (img)
	{
		return std::string(str->getCString());	
	}
	else
	{
		return std::string("building/740001.png");
	}
}

std::string BuildingCommon::getGrassNameFromGridCount( int count )
{
	auto result = std::string("building/3x3.png");
	switch (count)
	{
	case 4:
		result = std::string("building/4x4.png");
		break;
	case 5:
		result = std::string("building/5x5.png");
		break;
	default:
		break;
	}
	return result;
}




UIPanZoomLayer * BuildingCommon::getPlayer3(const std::vector<DBuilding*> db, std::vector<BattleLogic*> & mapOut )
{
	auto pzLayer = UIPanZoomLayer::create();
	pzLayer->setMap(getMainMap(),true);
	pzLayer->setScale( 0.5f );
	for (auto item: db)
	{   
		auto build = UIBuildingFactory::CreateBuildingFromBattleLogic(item);
		pzLayer->addBuilding(build,(MAX_GRID_COUNT  * item->coord.y) +  item->coord.x);
		mapOut.push_back(build);
	}
	return pzLayer;
}

Layer * BuildingCommon::getMainMap()
{

	auto layer = Layer::create();
	layer->setContentSize(Size(4608,2548));

	auto sky01 = Sprite::create("background/sky01.jpg");
	sky01->setAnchorPoint(Point(1,0));
	sky01->setPosition(4608,0);
	layer->addChild(sky01);

	auto sky02 = Sprite::create("background/sky02.jpg");
	sky02->setAnchorPoint(Point(0,1));
	sky02->setPosition(0,2548);
	layer->addChild(sky02);

	auto sky03 = Sprite::create("background/sky03.jpg");
	sky03->setAnchorPoint(Point(0,1));
	sky03->setPosition(2048,2548);
	layer->addChild(sky03);

	auto sky04 = Sprite::create("background/sky04.jpg");
	sky04->setAnchorPoint(Point(1,1));
	sky04->setPosition(4608,2548);
	layer->addChild(sky04);

	auto home01a = Sprite::create("background/home01a.png");
	home01a->setAnchorPoint(Point::ZERO);
	home01a->setPosition(Point::ZERO);
	layer->addChild(home01a);

	auto home01b = Sprite::create("background/home01b.png");
	home01b->setAnchorPoint(Point::ZERO);
	home01b->setPosition(Point(1024,0));
	layer->addChild(home01b);

	auto home02 = Sprite::create("background/home02.png");
	home02->setAnchorPoint(Point::ZERO);
	home02->setPosition(Point(2048,0));
	layer->addChild(home02);

	auto home03 = Sprite::create("background/home03.png");
	home03->setAnchorPoint(Point(0,1));
	home03->setPosition(Point(0,2548));
	layer->addChild(home03);

	auto home04 = Sprite::create("background/home04.png");
	home04->setAnchorPoint(Point(0,0));
	home04->setPosition(Point(2048,2548 - 500));
	layer->addChild(home04);

	layer->ignoreAnchorPointForPosition(false);
	layer->setAnchorPoint(Point(0.5f,0.5f));

	return layer;
}

std::string BuildingCommon::getAttackstring( int level )
{
	auto str = String::createWithFormat("%02d_attack",level);
	return str->getCString();
}



std::string BuildingCommon::getFlystring( int level )
{
	auto str = String::createWithFormat("%02d_fly",level);
	return str->getCString();
}

std::string BuildingCommon::getFlyTowerstring( int level,int degree )
{   
	if (degree <= 6)
	{
		auto str = String::createWithFormat("%02d_fly%02d",level,degree);
		return str->getCString();
	}
	else
	{
		return getFlystring(level);
	}
	
}
std::string BuildingCommon::getBoomstring( int level )
{   
	//TODO
	level = 1;
	auto str = String::createWithFormat("%02d_boom",level);
	return str->getCString();
}

static Point m_p1 = Point::ZERO;
static Point m_p2 = Point::ZERO;
Point BuildingCommon::getTmpPoint()
{
	return m_p1;
}

void BuildingCommon::setTmpPoint( Point p1,Point p2 )
{
    m_p1 = p1;
	m_p2 = p2;
}

Point BuildingCommon::getTmpPoint2()
{
	return m_p2;
}

std::string BuildingCommon::getHeroPicFromId( int id )
{
	auto str = String::createWithFormat("hero/hero01.png")->getCString();
	return str;
}

std::string BuildingCommon::getWallStr( int level,int state )
{
	auto str = String::createWithFormat("%02d_wall%02d",level,state);
	return str->getCString();
}







	

