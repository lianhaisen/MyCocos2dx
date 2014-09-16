#include "HeroConfig.h"
#include "SqliteHelper.h"

static map<int,HeroInfo*> m_herocfg;
map<int,HeroInfo*> HeroInfo::getHeroConfig()
{
	if (m_herocfg.empty())
	{
		initHeroConfig();
	}
	return m_herocfg;
}

HeroInfo* HeroInfo::getHeroFromId( int id )
{
	auto map = getHeroConfig();
	auto result = map.find(id);
	if (result != map.end())
	{
		return result->second;
	}
	return nullptr;
}

void HeroInfo::initHeroConfig()
{
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from c_heroinfo");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{
		auto item  = new HeroInfo();
		item->id = query.getIntField("id");
		item->race = query.getIntField("race");
		item->name = query.getStringField("name");
		item->attacktype = query.getIntField("attacktype");
		item->unittype = query.getIntField("unittype");
		item->attackpre = query.getIntField("attackpre");
		item->hurttype = query.getIntField("hurttype");
		item->AOErange = query.getIntField("AOErange");
		item->burstscount = query.getIntField("burstscount");
		item->attackmin = query.getIntField("attackmin");
		item->attackmax = query.getIntField("attackmax");
		item->attackspeed = query.getIntField("attackspeed");
		item->movespeed   = query.getIntField("movespeed");
		item->alertradius = query.getIntField("alertradius");
		item->population  = query.getIntField("population");
		item->energypoint = query.getIntField("energypoint");
		item->mainlv = query.getIntField("mainlv");
		item->energystone = query.getIntField("energystone");
		item->skillid = query.getIntField("skillid");
		item->skillname = query.getStringField("skillname");
		item->bulletflytime = query.getIntField("bulletflytime");

		m_herocfg[item->id] = item;
	    query.nextRow();
	}
	query.finalize();
}

static map<int,HeroBaseUpgrade*> m_herobaseupgrade;
map<int,HeroBaseUpgrade*> HeroBaseUpgrade::getHeroBaseUpgrade()
{
	if (m_herobaseupgrade.empty())
	{
		initHeroBaseUpgrade();
	}
	return m_herobaseupgrade;
}

HeroBaseUpgrade* HeroBaseUpgrade::getHeroBaseFromId( int id )
{
	auto map = getHeroBaseUpgrade();
	auto result = map.find(id);
	if (result != map.end())
	{
		return result->second;
	}
	return nullptr;
}

void HeroBaseUpgrade::initHeroBaseUpgrade()
{
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from c_herobaseupgrade");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{    
		auto item = new HeroBaseUpgrade();
		item->id = query.getIntField("id");
		item->costtype = query.getIntField("costtype");
		item->cost = query.getIntField("cost");
		item->costtiem_m = query.getIntField("costtiem_m");
		item->rebirthtime_m = query.getIntField("rebirthtime_m");
		item->honor = query.getIntField("honor");

		m_herobaseupgrade[item->id] = item;
		query.nextRow();
	}
	query.finalize();
}   

static map<int,HeroUpgrade*> m_heroupgrage;
map<int,HeroUpgrade*> HeroUpgrade::getHeroUpragrage()
{
	if (m_heroupgrage.empty())
	{
		initHeroUpragrade();
	}
	return m_heroupgrage;
}

HeroUpgrade* HeroUpgrade::getHeroUpgrageFromId( int id )
{
	auto map = getHeroUpragrage();
	auto result = map.find(id);
	if (result != map.end())
	{
		return result->second;
	}
	return nullptr;
}

void HeroUpgrade::initHeroUpragrade()
{
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from c_heroupgrade");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{    
		auto item = new HeroUpgrade();
		item->id = query.getIntField("id");
		item->type = query.getIntField("typeid");
		item->name = query.getStringField("name");
		item->hurt = query.getIntField("hurt");
		item->hp = query.getIntField("hp");
		
		m_heroupgrage[item->id] = item;
		query.nextRow();
	}
	query.finalize();
}

HeroUpgrade::HeroUpgrade()
{
	_baseinfo = nullptr;
}

HeroUpgrade::~HeroUpgrade()
{

}

HeroBaseUpgrade * HeroUpgrade::getBaseInfo()
{
	if (!_baseinfo)
	{
		_baseinfo = HeroBaseUpgrade::getHeroBaseFromId(type % 100);
	}
	return _baseinfo;
}

static map<int,HeroSkillUpgrade*> m_heroskillupgrade;
map<int,HeroSkillUpgrade*> HeroSkillUpgrade::getHeroSkillUpgrade()
{
	if (m_heroskillupgrade.empty())
	{
		initHeroSkillUpgrade();
	}
	return m_heroskillupgrade;
}

HeroSkillUpgrade* HeroSkillUpgrade::getHeroSkillFromId(int id)
{
	auto map = getHeroSkillUpgrade();
	auto result = map.find(id);
	if (result != map.end())
	{
		return result->second;
	}
	return nullptr;
}

void HeroSkillUpgrade::initHeroSkillUpgrade()
{
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from c_heroskillupgrade");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{
		auto item = new HeroSkillUpgrade();
		item->id  = query.getIntField("id");
		item->type = query.getIntField("typeid");
		item->name = query.getStringField("name");
		item->costtype = query.getIntField("costtype");
		item->cost = query.getIntField("cost");
		item->costtime = query.getIntField("costtime");
		item->effecttime = query.getIntField("effecttime");
		item->attackadd = query.getIntField("attackadd");
		item->moveadd = query.getIntField("moveadd");
		item->blowhurt = query.getIntField("blowhurt");
		item->turnedloseinjury = query.getIntField("turnedloseinjury");
		item->treat = query.getIntField("treat");
		item->treatcount = query.getIntField("treatcount");
		item->treatinterval = query.getIntField("treatinterval");
		item->reduceattackspeed = query.getIntField("reduceattackspeed");
		m_heroskillupgrade[item->id] = item;
		query.nextRow();
	}
	query.finalize();



}

static map<int,HeroLevelAndSkillLevel*> m_heroandskill;
//是否有740009001的消息
static bool _boolen = false;
map<int,HeroLevelAndSkillLevel*> HeroLevelAndSkillLevel::getHeroLevelAndSkillLevel()
{
	if (m_heroandskill.empty())
	{
		init();
		if(!_boolen)
		{
			auto szSQL = String::createWithFormat("INSERT INTO d_upgrade(id,type,status,timestamp) VALUES(%d,0,0,0)",740009001);
			TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
			sqliteHandler->execDML(szSQL->getCString());
			_boolen = true;
		}
	}
	return m_heroandskill;
}

HeroLevelAndSkillLevel* HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(int id)
{
	auto map = getHeroLevelAndSkillLevel();
	auto pair = map.find(id);
	if (pair != map.end())
	{
		return pair->second;
	}
	return nullptr;
}

void HeroLevelAndSkillLevel::init()
{
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from d_level where id=740009001");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{
		auto item   =  new HeroLevelAndSkillLevel();
		item->id    =  query.getIntField("type");
		item->level =  query.getIntField("level");
		item->getRealLevel();
		m_heroandskill[item->id] = item;
		query.nextRow();
	}
	query.finalize();

	strSQL = std::string("select * from d_upgrade where id=740009001");
	query  = sqliteHandler->execQuery(strSQL.c_str());
	if(!query.eof())
	{
		_boolen = true;
		int tmpid = query.getIntField("type");
		int tmpstatus = query.getIntField("status");
		long long timestamp = query.getInt64Field("timestamp");
		auto pair = m_heroandskill.find(tmpid);
		if (pair != m_heroandskill.end())
		{ 
			//英雄升级
			auto item = pair->second;
			item->status = tmpstatus;
			item->timestamp = timestamp;
		}
		else
		{
			//技能升级
			for (auto pair : m_heroandskill)
			{
				auto item     = pair.second;
				auto heroinfo =  HeroInfo::getHeroFromId(item->id);
				if (heroinfo->skillid == tmpid)
				{   
					item->isupgradehero = 1;
					item->status = tmpstatus;
					item->timestamp = timestamp;
					break;
				}
			}

		}
	}
    query.finalize();
}

void HeroLevelAndSkillLevel::getRealLevel()
{
	skilllevel = level % 1000;
	herolevel  = (level / 10000) % 1000;
}

void HeroLevelAndSkillLevel::getToLevel()
{
	level = (1000 + herolevel) * 10000 + (1000 + skilllevel);
}

HeroLevelAndSkillLevel::HeroLevelAndSkillLevel()
{   
	isupgradehero = 0;
	status = 0;
	timestamp = 0;
}

void HeroLevelAndSkillLevel::createHeroItem( int nType,int nLevel /*= 1*/ )
{
	auto map = getHeroLevelAndSkillLevel();
	auto pair = map.find(nType);
	if (pair != map.end())
	{
		return ;
	}
	else
	{
		auto item   =  new HeroLevelAndSkillLevel();
		item->id    =  nType;
		//item->getRealLevel();
		item->isupgradehero = 0;
		item->status = 0;
		item->timestamp = 0;
		item->herolevel = 1;
		item->skilllevel = 1 ;
		item->getToLevel();
		

		auto szSQL = String::createWithFormat("INSERT INTO d_level(id,type,level) VALUES(740009001,%d,%d)",item->id,item->level);
		TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
		sqliteHandler->execDML(szSQL->getCString());

		m_heroandskill[item->id] = item;
	}

}

int HeroLevelAndSkillLevel::getTotalTime()
{		
	time_t tt;
	time(&tt);
	if (tt < this->timestamp)
	{
		return (int)(this->timestamp - tt);
	}
	if(this->isupgradehero == 0 && this->status == 1){
		this->herolevel++;
	}
	else if(this->isupgradehero == 1 && this->status == 1 )
	{
		this->skilllevel++;
	}
	this->getToLevel();
	//更新等级列表数据
	auto szSql = String::createWithFormat("UPDATE d_level SET level=%d WHERE type=%d",this->level,this->id)->getCString();
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSql);



	//更新升级更新表
	this->status = 0;
	szSql = String::createWithFormat("UPDATE d_upgrade SET status=%d,timestamp=%lld WHERE id=740009001",this->status,this->timestamp)->getCString();
	TSQLite3DB *sqliteHandler2 = SqliteHelper::getSqliteHandler();
	sqliteHandler2->execDML(szSql);
	return 0;
}

bool HeroLevelAndSkillLevel::setUpData(int nUpgradeHero)
{
	auto result = false;
	auto item = m_heroandskill.begin();
	for(item;item != m_heroandskill.end() ;item++)
	{
		if(item->second->status == 1 )
			return result;
	}
	this->isupgradehero = nUpgradeHero;
	if(this->herolevel == 0) this->herolevel+=1;
	//所需时间
	int nTimestamp = 0;
	if(HeroBaseUpgrade::getHeroBaseFromId(this->herolevel+1)){
		nTimestamp = HeroBaseUpgrade::getHeroBaseFromId(this->herolevel+1)->costtiem_m*60;
	}
	else {
		return result;
	}
	//所升级的ID 
	int nId = this->id;
	if(nUpgradeHero == 1 && HeroInfo::getHeroFromId(this->id) )
	{
		nId = HeroInfo::getHeroFromId(this->id)->skillid;
	}
	else if(!HeroInfo::getHeroFromId(this->id)){
		return result;
	}
	time_t tt;
	time(&tt);
	this->timestamp = tt + nTimestamp ;
	this->status = 1;

	auto szSql = String::createWithFormat("UPDATE d_upgrade SET type=%d,status=%d,timestamp=%lld WHERE id=740009001",nId,this->status,this->timestamp)->getCString();
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSql);

	result = true;

	return result;

}




/*                 英雄祭坛配置信息                        */
static map<int,HeroAltarConfig*>m_mHeroAltarConfig;
HeroAltarConfig::HeroAltarConfig()
{
	id = 0 ;
	type = 0 ;
}

void HeroAltarConfig::init()
{
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from d_heroaltarconfig");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{
		auto item = new HeroAltarConfig();
		item->id  = query.getIntField("id");
		item->type = query.getIntField("type");

		query.nextRow();
		m_mHeroAltarConfig[item->id] = item;
	}
	query.finalize();
}

map<int,HeroAltarConfig*> HeroAltarConfig::getHeroAltarConfig()
{
	if (m_mHeroAltarConfig.empty())
	{
		init();
	}
	return m_mHeroAltarConfig;
}

HeroAltarConfig* HeroAltarConfig::getHeroAltarConfigFromId( int id )
{
	auto map = getHeroAltarConfig();
	auto pair = map.find(id);
	if (pair != map.end())
	{
		return pair->second;
	}
	return nullptr;
}

void HeroAltarConfig::UpdataHeroToAltar( int nType )
{

	this->type = nType;

	auto szSql = String::createWithFormat("UPDATE d_heroaltarconfig SET type=%d WHERE id=%d",nType,this->id)->getCString();
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSql);
}

void HeroAltarConfig::createHeroToAltar( int nId,int nType )
{
	auto map = getHeroAltarConfig();
	auto pair = map.find(nId);
	if (pair != map.end())
	{
		return ;
	}
	else 
	{
		auto item   =  new HeroAltarConfig();
		item->id    =  nId;
		item->type = nType;

		auto szSQL = String::createWithFormat("INSERT INTO d_heroaltarconfig(id,type) VALUES(%d,%d)",item->id,item->type);
		TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
		sqliteHandler->execDML(szSQL->getCString());

		m_mHeroAltarConfig[item->id] = item;


	}

}
