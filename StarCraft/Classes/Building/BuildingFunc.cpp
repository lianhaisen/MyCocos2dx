#include "BuildingFunc.h"
#include "BuildingConfig.h"
#include "BuildingData.h"
#include "UIContainer.h"
static BuildingFunc * m_Instance = nullptr;

BuildingFunc * BuildingFunc::getInstance()
{   
	if (!m_Instance)
	{
		m_Instance = new BuildingFunc();
	}
	return m_Instance;
}

void BuildingFunc::addContains( DBuilding * db )
{
	auto type = db->type / 100;
	switch (type)
	{
	//蓝水晶
	case 740002:
		_blueContains.push_back(db);
		break;
	//金属
	case 740004:
		_goldContains.push_back(db);
		break;
	// 黑水晶
	case 740006:
		_blackContains.push_back(db);
		break;
	default:
		break;
	}

	//找到军营
	if (type == 740007 )
	{
		_camps.push_back(db);
	}

	//找到主建筑
	if (type == 740018 )
	{
		_mainBuilding = db;
	}

	//找到实验室
	if (type == 740010)
	{
		_science = db;
	}


	//找到兵营建筑
	switch (type)
	{
	case 740012:
	case 740013:
	case 740014:
		_barracks.push_back(db);
		break;
	default:
		break;
	}


}

void BuildingFunc::CollectResource( DBuilding * db )
{   
    auto container = getContainsFromType(db->type / 100);
	sort(container.begin(),container.end(),[](DBuilding * a,DBuilding * b){return (a->getCurBindCfg()->storage - a->curpro) < (b->getCurBindCfg()->storage - b->curpro); });
	auto left = getLeftCanStore(container);
	auto total = left < db->curpro ? left : db->curpro; 
	AddResourceToMainUI(db->type / 100,total);
	db->curpro -= total;
	db->getBuilding()->updateStatus();
	//最小剩余的先填满
	for (auto item : container)
	{
		auto tleft = item->getCurBindCfg()->storage - item->curpro;
		if (total < tleft)
		{   
		    item->curpro += total;
			item->updatePro();
		    total = 0;
			break;
		}
		else
		{
			item->curpro += tleft;
			item->updatePro();
			total -= tleft;
		}
		auto container = item->getBuilding();
		container->updateStatus();
	
	}
}

vector<DBuilding*> BuildingFunc::getContainsFromType( int type )
{   
	vector<DBuilding*> result ;
	switch (type)
	{
		//蓝水晶
	case 740001:
		result = _blueContains;
		break;
		//金属
	case 740003:
		result = _goldContains;
		break;
		// 黑水晶
	case 740005:
		result = _blackContains;
		break;
	default:
		break;
	}
	return result;
}

int BuildingFunc::getLeftCanStore( vector<DBuilding*> ve )
{
	int result = 0;
	for (auto item : ve)
	{
		result += item->getCurBindCfg()->storage - item->curpro;
	}
	return result;
}

BuildingFunc::BuildingFunc()
{
	_mainBuilding = nullptr;
	_science = nullptr;
}

BuildingFunc::~BuildingFunc()
{

}

int BuildingFunc::getMainLevel()
{   
	if (_mainBuilding)
	{
		return _mainBuilding->type % 100;
	}
	return 1;
}

void BuildingFunc::setMinUI( UIUserMinUi* ui )
{

	for (int i = 1; i <=3; i++)
	{    
		 string a;
		 vector<DBuilding*> ve;
		 if (i == 1)
		 {   
			 a = "blue";
			 ve = _blueContains;
		 }
		 else if (i == 2)
		 {   
			 a  = "gold";
			 ve = _goldContains;
		 }
		 else
		 {   
			 a  = "black";
			 ve = _blackContains;
		 }
		 auto cur = getCurStroe(ve);
		 auto max = getMaxCanStore(ve);
		 auto str = String::createWithFormat("%s:%d",a.c_str(),max);
		 log("cur:%d max:%d ",cur,max);
		 UIUserMinUi::getInstance()->InitRightUiNum(i+1,cur,max,str->getCString());

	}

	for (auto item : _barracks)
	{
	    getSolidListFromDBuilding(item);
	}

}

int BuildingFunc::getMaxCanStore( vector<DBuilding*> ve )
{
	int result = 0;
	for (auto item : ve)
	{
		result += item->getCurBindCfg()->storage;
	}
	return result;
}

int BuildingFunc::getCurStroe( vector<DBuilding*>ve )
{
	int result = 0;
	for (auto item : ve)
	{
		result += item->curpro;
	}
	return result;
}

void BuildingFunc::AddResourceToMainUI( int type,int count )
{

 	switch (type)
	{
		case 740001:
			UIUserMinUi::getInstance()->addRightUiNum(2,count);
			break;;

		case 740003:
			UIUserMinUi::getInstance()->addRightUiNum(3,count);
			break;
			// 黑水晶
		case 740005:
			UIUserMinUi::getInstance()->addRightUiNum(4,count);
			break;
		default:
			break;
	}
	
}

vector<CSoldier*> BuildingFunc::getSolidListFromDBuilding( DBuilding * db )
{   
	vector<CSoldier*> result; 

	if (_barracksBuildlist.empty())
	{
		//初始化
		auto soildcfglist = SoldierConfig::getSoldierConfig();
		for (auto item : soildcfglist)
		{     
			 auto soildcfg = item.second;
             for (auto building : _barracks)
             {    
				  auto usersoldierlevel = UserSoldierLevel::getUserSoldierLevelFromId(soildcfg->type);
				  if (soildcfg->building_id == building->type / 100 &&  soildcfg->id % 100 == usersoldierlevel->level)
				  {   

					  auto sitem = _barracksBuildlist.find(building);
					  if (sitem ==  _barracksBuildlist.end())
					  {
						  //没找到新建
						   vector<CSoldier*> newitem;
						   newitem.push_back(soildcfg);
						  _barracksBuildlist.insert(map<DBuilding*,vector<CSoldier*>>::value_type(building,newitem));
					  }
					  else
					  {
						  sitem->second.push_back(soildcfg);
					  }
					 
				  }
             }
		}
	}

	auto re = _barracksBuildlist.find(db);
	if (re != _barracksBuildlist.end())
	{
		return re->second;
	}
	else
	{
		return result;
	}

}


map<int,BarrackItem*> BuildingFunc::getAllSoldier()
{
	if (_allSoldiers.empty())
	{
		for (auto item: BarrackData::getBarrackDataMap())
		{
			auto bdata = item.second;
			for (auto sitem : bdata->soldieritems)
			{
			     auto findpair = _allSoldiers.find(sitem->soldierid);
				 BarrackItem * finditem = nullptr;
				 if (findpair == _allSoldiers.end())
				 {
					  auto newitem = new BarrackItem();
					  newitem->soldierid = sitem->soldierid;
					  newitem->number = 0;
					  newitem->soldierstate = 0;
					  newitem->population = sitem->population;
					  _allSoldiers[sitem->soldierid] = newitem;
					  finditem = newitem;
				 }
				 else 
				 {
					 finditem = findpair->second;
				 }
				 finditem->number += sitem->number;
				 finditem->soldierstate += sitem->soldierstate;
			}
		}
	}
	return _allSoldiers;
}



int BuildingFunc::getScienceLevel()
{
	if (_science)
	{
		return _science->type % 100;
	}
	return 1;
}

void BuildingFunc::addSoldier( Coord pos,int id,int popution )
{    
	if (_camps.empty())
	{
		return;
	}

	//排序
	sort(_camps.begin(),_camps.end(),[=](DBuilding * a,DBuilding * b){return  (pos.x - a->coord.x)*(pos.x - a->coord.x) + (pos.y - a->coord.y)*(pos.y - a->coord.y)
		<   (pos.x - b->coord.x)*(pos.x - b->coord.x) + (pos.y - b->coord.y)*(pos.y - b->coord.y);});
	
	//TODO 分配兵力
	for (auto item : _camps)
	{
		auto sitem = BarrackData::getBarrackFromId(item->id);
		if (sitem->maxpopulation - sitem->curpopulation >= popution)
		{   
			sitem->curpopulation += popution;
			sitem->addSoldier(id);
			sitem->updateData();
			break;
		}
	}
	for (auto item :_allSoldiers)
	{
		delete item.second;
	}

	_allSoldiers.clear();



}

DBuilding* BuildingFunc::getBarrackFromIndex( int index )
{
	int i = 1;
	DBuilding * resutl = nullptr; 
	for (auto item : _barracks)
	{
		if (i == index)
		{   
			resutl = item;
			break;
		}
		i++;
	}
	return resutl;
}

int BuildingFunc::getBarrackIndexFormDB( DBuilding * db )
{  
	int i = 1;
	for (auto item : _barracks)
	{
		if (db == item)
		{   
			break;
		}
		i++;
	}
	return i;
}

