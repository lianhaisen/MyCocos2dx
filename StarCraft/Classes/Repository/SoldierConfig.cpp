#include "SoldierConfig.h"
#include "SqliteHelper.h"


static map<int, CSoldier*> s_SoldierConfig;

void SoldierConfig::initSoldierConfig()
{
	s_SoldierConfig.clear();

	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from c_soldier");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{
		CSoldier *item = new CSoldier();
		item->id = query.getIntField("id");
		item->name = query.getStringField("name");
		item->type = query.getIntField("type");
		//item->level = query.getIntField("level");
		item->attack_type = query.getIntField("attack_type");
		item->preference = query.getIntField("preference");
		item->attack_target = query.getIntField("attack_target");
		item->aoe_range = (float)query.getIntField("aoe_range")/10;
		item->min_range = query.getIntField("min_range");
		item->max_range = query.getIntField("max_range");
		item->attack_speed = (float)query.getIntField("attack_speed")/1000;
		item->attack = query.getIntField("attack");
		item->max_hp = query.getIntField("max_hp");
		item->dot = query.getIntField("dot");
		item->dot_time = query.getIntField("dot_time");
		item->move_speed = (float)query.getIntField("move_speed")/10;
		item->population = query.getIntField("population");
		item->air_population = query.getIntField("air_population");
		item->train_time = query.getIntField("train_time");
		item->is_air = query.getIntField("is_air");
		item->is_invisible = query.getIntField("is_invisible");
		item->is_jump = query.getIntField("is_jump");
		item->camp_level = query.getIntField("camp_level");
		item->lab_level = query.getIntField("lab_level");
		item->building_id = query.getIntField("building_id");
		item->skill_type = query.getIntField("skill_type");
		item->cost_type = query.getIntField("cost_type");
		item->train_cost = query.getIntField("train_cost");
		item->upgrade_cost = query.getIntField("upgrade_cost");
		item->upgrade_time = query.getIntField("upgrade_time");
		item->race = query.getIntField("race");
		item->power_points = (float)query.getIntField("power_points")/10;
		item->chip = query.getIntField("chip");
		item->unit_type = query.getIntField("unit_type");
		item->life_type = query.getIntField("life_type");
		item->soldier_points = query.getIntField("soldier_points");
		item->fly_speed = (float)query.getIntField("fly_speed")/10;
		s_SoldierConfig[item->id] = item;
		query.nextRow();
	}
	query.finalize();
}

map<int, CSoldier*> SoldierConfig::getSoldierConfig()
{
	if (s_SoldierConfig.empty())
	{
		initSoldierConfig();
	}
	
	return s_SoldierConfig;
}

CSoldier* SoldierConfig::getSoldierConfig(int id)
{   
	auto map = getSoldierConfig();
	auto result = map.find(id);
	if (result != map.end())
	{
		return result->second;
	}
	return nullptr;
	
}


static map<int,UserSoldierLevel*> m_usersoldierleves;
UserSoldierLevel::UserSoldierLevel()
{
	id = 0;
	level = 0;
	_curcfg = nullptr;
	_nextcfg = nullptr;
}

map<int,UserSoldierLevel*> UserSoldierLevel::getUserSoldierLevels()
{
	if (m_usersoldierleves.empty())
	{
		initSoldierLevels();
	}
	return m_usersoldierleves;
}

void UserSoldierLevel::initSoldierLevels()
{
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from d_level where id=740010001");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{
		auto item   =  new UserSoldierLevel();
		item->id    =  query.getIntField("type");
		item->level =  query.getIntField("level");
		m_usersoldierleves[item->id] = item;
		query.nextRow();
	}
	query.finalize();
}

UserSoldierLevel * UserSoldierLevel::getUserSoldierLevelFromId( int id )
{
	auto map = getUserSoldierLevels();
	auto result = map.find(id);
	if (result != map.end())
	{
		return result->second;
	}
	return nullptr;

}

CSoldier * UserSoldierLevel::getCurcfg()
{
	if (_curcfg == nullptr || _curcfg->id % 100 != level)
	{   
		auto str = String::createWithFormat("%d%02d",id,level);
		auto value = Value(str->getCString()).asInt();
		_curcfg = SoldierConfig::getSoldierConfig(value);
	}
	return _curcfg;
}

CSoldier * UserSoldierLevel::getNextcfg()
{
	if (_nextcfg == nullptr || _nextcfg->id % 100 != level + 1)
	{   
		auto str = String::createWithFormat("%d%02d",id,level+1);
		auto value = Value(str->getCString()).asInt();
		_nextcfg = SoldierConfig::getSoldierConfig(value);
	}
	return _nextcfg;
}

void UserSoldierLevel::updateLevel()
{
	level += 1;
	auto szSQL = String::createWithFormat("UPDATE d_level SET level=%d WHERE type=%d and id=740010001",this->level,this->id);
	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	sqliteHandler->execDML(szSQL->getCString());
}
