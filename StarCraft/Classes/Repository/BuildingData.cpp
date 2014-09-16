#include "BuildingData.h"
#include "SqliteHelper.h"
#include "BuildingConfig.h"
#include "BuildingFunc.h"

static vector<DBuilding*> s_BuildingData;

void BuildingData::initBuildingData()
{
	s_BuildingData.clear();

	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from d_building");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());

	while (!query.eof())
	{
		DBuilding *item = new DBuilding();
		item->id = query.getIntField("id");
		item->coord.x = query.getIntField("x");
		item->coord.y = query.getIntField("y");
		item->status = (DBuilding::BuildingStatus)query.getIntField("status");
		item->type = query.getIntField("typeid");
		item->timestamp = query.getInt64Field("timestamp");
		item->curpro = query.getIntField("curpro");
		s_BuildingData.push_back(item);
        BuildingFunc::getInstance()->addContains(item);
		query.nextRow();
	}

	sort(s_BuildingData.begin(),s_BuildingData.end(),[](DBuilding * a,DBuilding * b){return a->coord.y < b->coord.y;});

	query.finalize();
}

vector<DBuilding*> BuildingData::getBuildingData()
{
	if (s_BuildingData.empty())
	{
		initBuildingData();
	}

	return s_BuildingData;
}

DBuilding* BuildingData::getBuildingData(int id)
{
	if (s_BuildingData.empty())
	{
		initBuildingData();
	}
	DBuilding *item = s_BuildingData.at(id - 1);
	return item;
}


DBuilding* BuildingData::CreateDBuilding( int type,int x,int y )
{   
	auto buycfg = BuyListConfig::getBuyListFromId(type /100);
	buycfg->current++;
	auto str = String::createWithFormat("%d%03d",type/100,buycfg->current);

	DBuilding *item = new DBuilding();
	item->id =  atoi(str->getCString());
	item->coord.x =x;
	item->coord.y =y;
	item->type = type;
	item->timestamp = 0;
	item->curpro = 0;
	s_BuildingData.push_back(item);

	auto cfg = item->getCurBindCfg();
	if (cfg->cost_time > 0)
	{
		item->status = DBuilding::BuildingStatus::NewBuild;
		time_t tt;
		time(&tt);
		item->timestamp = tt + cfg->cost_time ;
	}
	else
	{
	    item->status = DBuilding::BuildingStatus::RELEX;
	}

	BuildingFunc::getInstance()->addContains(item);

	char szSQL[500];
	sprintf(szSQL, "INSERT INTO d_building(id,typeid,x,y,status,timestamp,curpro) VALUES(%d,%d,%d,%d,%d,%lld,%d)",item->id,item->type,item->coord.x,item->coord.y,item->status,item->timestamp,item->curpro);
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSQL);
	return item;
}


void DBuilding::updateCoor(int x,int y)
{

	this->coord.x = x;
	this->coord.y = y;

	char szSQL[500];
    sprintf(szSQL, "UPDATE d_building SET typeid=%d, x=%d ,y=%d, status=%d WHERE id=%d",this->type,this->coord.x,this->coord.y,this->status,this->id);
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSQL);
    

}

DBuilding::DBuilding()
{
	_curCfg = nullptr;
	_nextCfg = nullptr;
	_maxCfg = nullptr;
	_building = nullptr;
}

DBuilding::~DBuilding()
{

}

CBuilding * DBuilding::getCurBindCfg()
{
	if (_curCfg == nullptr || _curCfg->id != this->type)
	{
		_curCfg = BuildingConfig::getBuildingConfig(this->type);
	}
	return _curCfg;
}

CBuilding * DBuilding::getNextBindCfg()
{  
	if (_nextCfg == nullptr || _nextCfg->id != this->type + 1)
	{
		_nextCfg =  BuildingConfig::getBuildingConfig(this->type + 1);
	}
	return _nextCfg;
}

CBuilding * DBuilding::getMaxBindCfg()
{   
	if (_maxCfg == nullptr)
	{
		auto sid = this->type;
		auto temp = getCurBindCfg();
		while (temp != nullptr)
		{
			_maxCfg = temp;
			 temp = BuildingConfig::getBuildingConfig(++sid);
		}   
	}
	return _maxCfg;
}

bool DBuilding::setUpgrade()
{   
	auto result = false;
	if (status == BuildingStatus::RELEX || status == BuildingStatus::WORK)
	{   
		auto nextcfg = getNextBindCfg();
		if (nextcfg != nullptr)
		{
			time_t tt;
			time(&tt);
			this->timestamp = tt + nextcfg->cost_time ;
			this->status = BuildingStatus::UPGRADE;
			
			struct tm * now = localtime(&tt);
			log("%d/%d/%d %d:%d:%d",now->tm_year+1900,now->tm_mon+1,
				now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);


			char szSQL[500];
			sprintf(szSQL, "UPDATE d_building SET status=%d ,timestamp=%lld WHERE id=%d",(int)this->status,this->timestamp,this->id);
			TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
			sqliteHandler->execDML(szSQL);
			
			result = true;
		}
	}
	return result;
}

int DBuilding::getLeftTime()
{
	if (status == BuildingStatus::UPGRADE || status ==  BuildingStatus::NewBuild)
	{
		time_t tt;
		time(&tt);
		if (tt < this->timestamp)
		{
			return (int)(this->timestamp - tt);
		}
		else
		{
			if (status == BuildingStatus::UPGRADE)
			{
				this->type += 1;
			}
			this->status = BuildingStatus::RELEX;
             
			char szSQL[500];
			sprintf(szSQL, "UPDATE d_building SET status=%d ,typeid=%d WHERE id=%d",(int)this->status,this->type,this->id);
			TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
			sqliteHandler->execDML(szSQL);

			return 0;
		}
	} 
	return 0;
}

void DBuilding::updatePro()
{     
	 time_t tt;
	 time(&tt);
	 this->timestamp = tt;
     auto szSQL = String::createWithFormat("UPDATE d_building SET curpro=%d ,status=%d,timestamp=%lld WHERE id=%d",this->curpro,(int)this->status,this->timestamp,this->id);
	 TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	 sqliteHandler->execDML(szSQL->getCString());

}


