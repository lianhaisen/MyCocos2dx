#include "BarrackData.h"
#include "SqliteHelper.h"
#include <sstream>
#include <cstdlib>
#include "BuildingData.h"
#include "BuildingConfig.h"

static map<int,BarrackData*> m_barrackData;

map<int,BarrackData*> BarrackData::getBarrackDataMap()
{   
	if (m_barrackData.empty())
	{
		//初始化
		initBarrackData();
	}
	return m_barrackData;
}

BarrackData * BarrackData::getBarrackFromId( int id )
{
	auto map = getBarrackDataMap();
	auto result = map.find(id);
	if (result != map.end())
	{
		return result->second;
	}
	else
	{
		return createNew(id);
	}
    return nullptr;
}

void BarrackData::initBarrackData()
{
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from d_barrack");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{   
		auto item = new BarrackData();
		item->id  = query.getIntField("id");
		item->maxpopulation  = query.getIntField("maxpopulation");
		item->curpopulation  = query.getIntField("curpopulation");
		item->soldiertypeids = query.getStringField("soldiertypeids"); 
		item->soldiernums    = query.getStringField("soldiernums");
		item->soldiersharepopulations = query.getStringField("soldiersharepopulations"); 
		item->soldierstates = query.getStringField("soldierstates"); 

		stringstream s1(item->soldiertypeids);
		stringstream s2(item->soldiernums);
		stringstream s3(item->soldiersharepopulations);
		stringstream s4(item->soldierstates);
		string s1tmp;
		string s2tmp;
		string s3tmp;
		string s4tmp;
		while (getline(s1,s1tmp,','))
		{
			getline(s2,s2tmp,',');
			getline(s3,s3tmp,',');
			getline(s4,s4tmp,',');
			auto soldieritem = new BarrackItem();
			soldieritem->id = item->id;
			soldieritem->soldierid  = atoi(s1tmp.c_str());
			soldieritem->number  = atoi(s2tmp.c_str());
			soldieritem->population = atoi(s3tmp.c_str());
			soldieritem->soldierstate = atoi(s4tmp.c_str());
			item->soldieritems.push_back(soldieritem);
		}

		m_barrackData[item->id] = item;
		//TODO 数据整理

		query.nextRow();
	}
	query.finalize();
}

void BarrackData::updateData()
{   
	soldiertypeids.clear();
	soldiernums.clear();
	soldiersharepopulations.clear();
	soldierstates.clear();
	int i = 0;
	for (auto item:soldieritems)
	{    
		soldiertypeids += String::createWithFormat("%d",item->soldierid)->getCString();
		soldiernums += String::createWithFormat("%d",item->number)->getCString();
		soldiersharepopulations += String::createWithFormat("%d",item->population)->getCString();
		soldierstates += String::createWithFormat("%d",item->soldierstate)->getCString();
		if (i != soldieritems.size() - 1)
		{
			soldiertypeids += ',';
			soldiernums += ',';
			soldiersharepopulations += ',';
			soldierstates += ',';
		}
		i++;
	}
	auto szSQL = String::createWithFormat("UPDATE d_barrack SET maxpopulation=%d ,curpopulation=%d,soldiertypeids='%s',soldiernums='%s',soldiersharepopulations='%s',soldierstates='%s' WHERE id=%d",maxpopulation,curpopulation,soldiertypeids.c_str(),soldiernums.c_str(),soldiersharepopulations.c_str(),soldierstates.c_str(),id);
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSQL->getCString());
}

void BarrackData::addSoldier( int id )
{   
	BarrackItem * result = nullptr;
	for (auto item : soldieritems )
	{
		if (item->soldierid == id)
		{
			result = item;
		}
	}
	if (result)
	{
		result->number += 1;
		curpopulation += result->population;
	}
	else
	{
		result = new BarrackItem();
		result->soldierid = id;
		result->id = this->id;
		result->number = 1;
	    result->population = SoldierConfig::getSoldierConfig(id)->population;
		result->soldierstate = 0;
		curpopulation += result->population;
		soldieritems.push_back(result);
	}
	updateData();
}
//更新士兵使用状态
void BarrackData::updataSoldierUseState(int Soldierid,int Barrackid,int nNum)
{
	BarrackItem * result = nullptr;
	for (auto item : this->soldieritems )
	{
		if (item->soldierid == Soldierid && item->id == Barrackid)
		{
			result = item;
		}
	}
	if (result)
	{
		result->soldierstate += nNum;
		updateData();
	}
}

BarrackData * BarrackData::createNew( int id )
{
	if (id / 1000 == 740007)
	{
		auto item = new BarrackData();
		item->id = id;
		item->curpopulation = 0;

		DBuilding * data = nullptr;
		
		for (auto item :BuildingData::getBuildingData())
		{
		    if (item->id == id)
		    {
				data = item;
				break;
		    }
		}
		item->maxpopulation = data->getCurBindCfg()->population;
        
		auto szSQL = String::createWithFormat("INSERT INTO d_barrack(id,maxpopulation,curpopulation,soldiertypeids,soldiernums,soldiersharepopulations,soldierstates) VALUES(%d,%d,%d,'','','','')",item->id,item->maxpopulation,item->curpopulation);
		TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
		sqliteHandler->execDML(szSQL->getCString());

		m_barrackData[item->id] = item;
		return item;
	}
	return nullptr;
}


static map<int,TrainingData*> m_traindata;
void TrainingData::initTrainingData()
{
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from d_training");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{   
		auto item = new TrainingData();
		item->id  = query.getIntField("id");
		item->maxpopulation  = query.getIntField("maxpopulation");
		item->curpopulation  = query.getIntField("curpopulation");
		item->soldiertypeids = query.getStringField("soldiertypeids");
	    item->soldiernums    = query.getStringField("soldiernums");
		item->soldiertrainingtimes = query.getStringField("soldiertrainingtimes");
		item->soldiersharepopulations = query.getStringField("soldiersharepopulations");

		stringstream s1(item->soldiertypeids);
		stringstream s2(item->soldiernums);
		stringstream s3(item->soldiersharepopulations);
		stringstream s4(item->soldiertrainingtimes);
		string s1tmp;
		string s2tmp;
		string s3tmp;
		string s4tmp;
		while (getline(s1,s1tmp,','))
		{
			getline(s2,s2tmp,',');
			getline(s3,s3tmp,',');
			getline(s4,s4tmp,',');
			auto soldieritem = new TrainItem();
			soldieritem->soldierid  = atoi(s1tmp.c_str());
			soldieritem->number  = atoi(s2tmp.c_str());
			soldieritem->population = atoi(s3tmp.c_str());
			soldieritem->trainingtime = atoi(s4tmp.c_str());
			item->soldieritems.push_back(soldieritem);
		}

		item->curtraininglavetime = query.getIntField("curtraininglavetime");
        m_traindata[item->id] = item;
		query.nextRow();
	}
	query.finalize();
}

void TrainingData::updateData()
{
     
	soldiertypeids.clear();
	soldiernums.clear();
	soldiertrainingtimes.clear();
	soldiersharepopulations.clear();
	int i = 0;
	for (auto item:soldieritems)
	{    
		soldiertypeids += String::createWithFormat("%d",item->soldierid)->getCString();
		soldiernums += String::createWithFormat("%d",item->number)->getCString();
		soldiertrainingtimes += String::createWithFormat("%d",item->trainingtime)->getCString();
		soldiersharepopulations += String::createWithFormat("%d",item->population)->getCString();
		if (i != soldieritems.size() - 1)
		{
			soldiertypeids += ',';
			soldiernums += ',';
			soldiertrainingtimes += ',';
			soldiersharepopulations += ',';
		}
		i++;
	}
	auto szSQL = String::createWithFormat("UPDATE d_training SET maxpopulation=%d ,curpopulation=%d,curtraininglavetime=%d,soldiertypeids='%s',soldiernums='%s',soldiertrainingtimes='%s',soldiersharepopulations='%s' WHERE id=%d",maxpopulation,curpopulation,curtraininglavetime,soldiertypeids.c_str(),soldiernums.c_str(),soldiertrainingtimes.c_str(),soldiersharepopulations.c_str(),id);
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSQL->getCString());
}



map<int,TrainingData*> TrainingData::getTrainingDataMap()
{
	if (m_traindata.empty())
	{
		initTrainingData();
	}
	return m_traindata;
}

TrainingData * TrainingData::getTrainingDataFromId( int id )
{
	auto map = getTrainingDataMap();
	auto result = map.find(id);
	if (result != map.end())
	{
		return result->second;
	}
	else
	{
		return CreateItem(id);
	}
	return nullptr;
}

int TrainingData::getNextCanBuildTime()
{  
	if (!soldieritems.empty())
	{
		auto item = soldieritems.front();
		
		if (item->number == 1 && soldieritems.size() == 1 && curtraininglavetime == item->trainingtime)
		{
			return item->trainingtime;
		}
		if (item->number == 1 && soldieritems.size() == 1 &&  curtraininglavetime != item->trainingtime )
		{
			return 0;
		}
		else if (item->number > 1)
		{
			return item->trainingtime;
		}
		else
		{  
		   if (soldieritems.size() >= 2)
		   {
			    auto it = soldieritems.begin();
				it++;
				return (*it)->trainingtime;
		   }
		   else
		   {
			   return 0;
		   }
		}
	}
	return 0;
}

int TrainingData::getTotalTime()
{
	int totaltime = 0;
	int index = 0;
	for (auto item : soldieritems)
	{  
		if (index == 0)
		{
			totaltime += (item->number - 1) * item->trainingtime;
		}
		else
		{
		    totaltime += item->number * item->trainingtime;  
		}
		index++;
	}
	if (soldieritems.size() > 0)
	{
		totaltime += curtraininglavetime;
	}
	return totaltime;
}

void TrainingData::deletaitems()
{
	while (soldieritems.size() > 0)
	{
		auto item = soldieritems.front();
		soldieritems.pop_front();
		CC_SAFE_DELETE(item);
	}
}

TrainingData * TrainingData::CreateItem( int id )
{  

	auto item = new TrainingData();
	item->id = id;
	item->maxpopulation = 0;
	item->curpopulation = 0;
	item->curpopulation = 0;


	auto szSQL = String::createWithFormat("INSERT INTO d_training(id,maxpopulation,curpopulation,soldiertypeids,soldiernums,soldiertrainingtimes,soldiersharepopulations,curtraininglavetime) VALUES(%d,%d,%d,'','','','',0)",item->id,item->maxpopulation,item->curpopulation);
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSQL->getCString());

	 m_traindata[item->id] = item;
	return item;
}


static map<int,SoldierUpgradeData*>  m_soldierupgradedata;
map<int,SoldierUpgradeData*> SoldierUpgradeData::getSoldierUpgradeMap()
{
	if (m_soldierupgradedata.empty())
	{
		initSoldierUpgradeData();
	}
	return m_soldierupgradedata;
}

SoldierUpgradeData* SoldierUpgradeData::getSoldierUpgradeMapFromId( int id )
{
	auto map = getSoldierUpgradeMap();
	auto result = map.find(id);
	if (result != map.end())
	{   
		auto item = result->second;
		item->getLeftTime();
		return item;
	}
	return nullptr;
}

void SoldierUpgradeData::initSoldierUpgradeData()
{

	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from d_upgrade where id=740010001");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{   
		auto item = new SoldierUpgradeData();
		item->id =  query.getIntField("id");
		item->type = query.getIntField("type");
		item->status = query.getIntField("status");
		item->timestamp = query.getInt64Field("timestamp");
		m_soldierupgradedata[item->id] = item;
		query.nextRow();
	}
	query.finalize();
}

SoldierUpgradeData::SoldierUpgradeData()
{
	_userLevel = nullptr;
}

SoldierUpgradeData::~SoldierUpgradeData()
{

}

UserSoldierLevel * SoldierUpgradeData::getSoldierLevel()
{
	if (status == 1 && _userLevel == nullptr)
	{
		_userLevel = UserSoldierLevel::getUserSoldierLevelFromId(type);
	}
	else if (status == 0)
	{
		_userLevel = nullptr;
	}
	return _userLevel;
}

int SoldierUpgradeData::getLeftTime()
{
	if (status == 1)
	{
		time_t tt;
		time(&tt);
		if (tt < this->timestamp)
		{
			return (int)(this->timestamp - tt);
		}
		else
		{  
			//TODO 更新等级列表,更新数据状态
			getSoldierLevel()->updateLevel();

			this->status = 0;
            auto szSql = String::createWithFormat("UPDATE d_soilderupgrade SET status=0 WHERE id=%d",this->id);
			TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
			sqliteHandler->execDML(szSql->getCString());

			return 0;
		}
	}
	return 0;
}

void SoldierUpgradeData::UpgradeSoldier( int bid,int soldiertype,int stime )
{
	 auto item = getSoldierUpgradeMapFromId(bid);
	 if (item == nullptr)
	 {
		 item = new SoldierUpgradeData();
		 item->id = bid;
		 item->type = 0;
		 item->status = 0;
		 item->timestamp = 0;
		 m_soldierupgradedata[item->id] = item;
		 //TODO 新建表单

		 auto szSQL = String::createWithFormat("INSERT INTO d_upgrade(id,type,status,timestamp) VALUES(%d,0,0,0)",item->id);
		 TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
		 sqliteHandler->execDML(szSQL->getCString());
	 }
	 item->status = 1;
	 item->type = soldiertype;
	 time_t tt;
	 time(&tt);
	 item->timestamp =  tt + stime;
     
	 auto szSQL = String::createWithFormat("UPDATE d_upgrade SET status=1 ,type=%d ,timestamp=%lld WHERE id=%d",item->type,item->timestamp,item->id);
	 TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	 sqliteHandler->execDML(szSQL->getCString());
}




//运输机
static map<int,ConveyorConfig*>  m_conveyorConfig;
void ConveyorConfig::initConveyorConfig()
{
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from d_conveyor");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{   
		auto item = new ConveyorConfig();
		item->id  = query.getIntField("id");
		item->barrackIds  = query.getStringField("barrackids");
		item->soldiertypeids = query.getStringField("soldiertypeids");
		item->soldiernums    = query.getStringField("soldiernums");
		stringstream s1(item->barrackIds);
		stringstream s2(item->soldiertypeids);
		stringstream s3(item->soldiernums);
		string s1tmp;
		string s2tmp;
		string s3tmp;
		while (getline(s1,s1tmp,','))
		{
			getline(s2,s2tmp,',');
			getline(s3,s3tmp,',');
			auto soldieritem = new BarrackItem();
			soldieritem->id = atoi(s1tmp.c_str());
			soldieritem->soldierid  = atoi(s2tmp.c_str());
			soldieritem->number  = atoi(s3tmp.c_str());
			item->soldieritems.push_back(soldieritem);
		}

		m_conveyorConfig[item->id] = item;
		query.nextRow();
	}
	query.finalize();
}


map<int,ConveyorConfig*> ConveyorConfig::getConveyorConfig()
{
	if (m_conveyorConfig.empty())
	{
		initConveyorConfig();
	}
	return m_conveyorConfig;
}

ConveyorConfig* ConveyorConfig::getConveyorConfigFromId( int id )
{
	auto map = getConveyorConfig();
	auto result = map.find(id);
	if (result != map.end())
	{   
		auto item = result->second;
		return item;
	}
	return nullptr;
}

void ConveyorConfig::CreateItem( int id )
{
	auto map = getConveyorConfig();
	auto result = map.find(id);
	if (result != map.end())
	{   
		return ;
	}

	auto item = new ConveyorConfig();
	item->id = id;
	item->soldierAllNum = 0;//运输机 人口容量

	auto szSQL = String::createWithFormat("INSERT INTO d_conveyor(id,barrackids,soldiertypeids,soldiernums) VALUES(%d,'','','')",item->id);
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSQL->getCString());

	m_conveyorConfig[item->id] = item;


}
void ConveyorConfig::updateData()
{
	barrackIds.clear();
	soldiertypeids.clear();
	soldiernums.clear();
	int i = 0;
	for (auto item:soldieritems)
	{    
		barrackIds += String::createWithFormat("%d",item->id)->getCString();
		soldiertypeids += String::createWithFormat("%d",item->soldierid)->getCString();
		soldiernums += String::createWithFormat("%d",item->number)->getCString();
		if (i != soldieritems.size() - 1)
		{
			barrackIds += ',';
			soldiertypeids += ',';
			soldiernums += ',';
		}
		i++;
	}
	auto szSQL = String::createWithFormat("UPDATE d_conveyor SET barrackids='%s' ,soldiertypeids='%s',soldiernums='%s' WHERE id=%d",barrackIds.c_str(),soldiertypeids.c_str(),soldiernums.c_str(),id);
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSQL->getCString());
}

void ConveyorConfig::addSoldier(int barrackId ,int soldierid )
{
	auto boolen = false;

	for (auto item: this->soldieritems)
	{
		if(item->soldierid == soldierid && item->id == barrackId)
		{
			item->number++;
			this->soldierAllNum += item->population;
			boolen = true;
			break;
		}
	}
	if(!boolen)
	{
		auto TmpItem = new BarrackItem();
		TmpItem->id = barrackId;
		TmpItem->soldierid = soldierid;
		TmpItem->number = 1;
		TmpItem->population  = SoldierConfig::getSoldierConfig(soldierid)->population;
		this->soldierAllNum += TmpItem->population;
		soldieritems.push_back(TmpItem);
	}

	updateData();
}

void ConveyorConfig::DeleteSoldier(int barrackId, int soldierid )
{
	for (auto item: this->soldieritems)
	{
		if(item->soldierid == soldierid && item->id == barrackId )
		{
			if(item->number >= 1){
				item->number--;
				this->soldierAllNum -= item->population;
				break;
			}
			else
			{
				this->soldieritems.remove(item);
				break;
			}
		}
	}
	updateData();

}
