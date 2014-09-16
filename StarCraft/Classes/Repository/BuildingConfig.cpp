#include "BuildingConfig.h"
#include "SqliteHelper.h"
#include "BuildingData.h"


static map<int, CBuilding*> s_BuildingConfig;

void BuildingConfig::initBuildingConfig()
{
	s_BuildingConfig.clear();

	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from c_building");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{
		CBuilding *item = new CBuilding();
		item->id = query.getIntField("id");
		item->name = query.getStringField("name");
		item->type = query.getIntField("typeid");
		item->grid = query.getIntField("grid");
		item->level = query.getIntField("level");
		item->townhall = query.getIntField("townhall");
		item->cost_time = query.getIntField("cost_time");
		item->cost_type = (CBuilding::ResourceType)query.getIntField("cost_type");
		item->cost_value = query.getIntField("cost_value");
		item->max_hp = query.getIntField("max_hp");
		item->attack = query.getIntField("attack");
		item->attack_target = query.getIntField("attack_target");
		item->preference = query.getIntField("preference");
		item->attack_type = query.getIntField("attack_type");
		item->aoe_range = query.getIntField("aoe_range");
		item->min_range = query.getIntField("min_range");
		item->max_range = query.getIntField("max_range");
		item->attack_speed = query.getIntField("attack_speed");
		item->defense_points = query.getIntField("defense_points");
		item->sub_attack = query.getIntField("sub_attack");
		item->add_attack = query.getIntField("add_attack");
		item->cost_repair = query.getIntField("cost_repair");
		item->trap_type = query.getIntField("trap_type");
		item->trap_effect = query.getIntField("trap_effect");
		item->attack_range = query.getIntField("attack_range");
		item->trigger_range = query.getIntField("trigger_range");
		item->trap_delay = query.getIntField("trap_delay");
		item->slow_time = query.getIntField("slow_time");
		item->slow_speed = query.getIntField("slow_speed");
		item->stun_time = query.getIntField("stun_time");
		item->freeze_time = query.getIntField("freeze_time");
		item->smoke_time = query.getIntField("smoke_time");
		item->poison_time = query.getIntField("poison_time");
		item->poison_attack = query.getIntField("poison_attack");
		item->alert_range = query.getIntField("alert_range");
		item->storage_type = query.getIntField("storage_type");
		item->storage = query.getIntField("storage");
		item->output = query.getIntField("output");
		item->population = query.getIntField("population");
		item->race = query.getIntField("race");
		item->power_points = query.getIntField("power_points");
		item->command = query.getIntField("command");
		item->exp = query.getIntField("exp");
		item->building_type = query.getIntField("building_type");
		item->fly_speed = (float)query.getIntField("fly_speed")/10;
		
		s_BuildingConfig[item->id] = item;
		query.nextRow();
	}
	query.finalize();
}

map<int, CBuilding*> BuildingConfig::getBuildingConfig()
{
	if (s_BuildingConfig.empty())
	{
		initBuildingConfig();
	}
	
	return s_BuildingConfig;
}

CBuilding* BuildingConfig::getBuildingConfig(int id)
{
	if (s_BuildingConfig.empty())
	{
		initBuildingConfig();
	}
	auto it = s_BuildingConfig.find(id);
	if (it == s_BuildingConfig.end())
	{
		return nullptr;
	}
	else
	{
		CBuilding *item = it->second;
		return item;
	}
}



BuyListConfig::BuyListConfig( CBuilding* scfg )
{
	buildcfg = scfg;
	current = 0;
	opencfg = nullptr;
}

BuyListConfig::BuyListConfig()
{

}

std::vector<BuyListConfig*> m_buylist1;
std::vector<BuyListConfig*> m_buylist2;
std::vector<BuyListConfig*> m_buylist3;
std::vector<BuyListConfig*> m_buylist4;
std::map<int,BuyListConfig*> m_configlist;

BuyListConfig * BuyListConfig::getBuyListFromId( int id )
{
	auto map = m_configlist;
	if (m_configlist.empty())
	{
		initBuyList();
	}
	auto result = map.find(id);
	if (result != map.end())
	{
		return result->second;
	}
	return nullptr;
}

std::vector<BuyListConfig*> BuyListConfig::getBuyListFromType( OpenBuilding::BuyBuildingType type /*= OpenBuilding::BuyBuildingType::FONDATION*/ )
{
	if (m_buylist1.empty())
	{
		initBuyList();
	}
	switch (type)
	{
		case OpenBuilding::BuyBuildingType::FONDATION:
			return m_buylist1;
		case OpenBuilding::BuyBuildingType::DEFENSE:
			return m_buylist2;
		case OpenBuilding::BuyBuildingType::RESOURCE:
			return m_buylist3;
		case OpenBuilding::BuyBuildingType::TRAP:
			return m_buylist4;
		default:
			break;
	}
	return m_buylist1;
}

void BuyListConfig::initBuyList()
{
	auto cbuildings = BuildingConfig::getBuildingConfig();
    
    std::map<int,BuyListConfig*> templist;

	for (auto item : cbuildings)
	{   
		//取第一级的
		if (item.first % 100 == 1)
		{
			templist[item.first/100] = new BuyListConfig(item.second);
		}
	}

    auto dbuildings = BuildingData::getBuildingData();
	for (auto item : dbuildings)
	{
		auto sitem = templist.find(item->type / 100);
		if (sitem != templist.end())
		{
			auto buycfg = sitem->second;
			buycfg->current += 1;
		}
	}

	auto openlist = OpenBuilding::getData();
	for (auto item : openlist)
	{   
		auto sitem = templist.find(item->id);
		if (sitem != templist.end())
		{
			auto buycfg = sitem->second;
			buycfg->opencfg = item;

			m_configlist[buycfg->opencfg->id] = buycfg;

			switch (item->type)
			{
			case OpenBuilding::BuyBuildingType::FONDATION:
				m_buylist1.push_back(buycfg);
				break;
			case OpenBuilding::BuyBuildingType::DEFENSE :
				m_buylist2.push_back(buycfg);
				break;
			case OpenBuilding::BuyBuildingType::RESOURCE:
				m_buylist3.push_back(buycfg);
				break;
			case OpenBuilding::BuyBuildingType::TRAP:
				m_buylist4.push_back(buycfg);
				break;
			default:
				break;
			}
		}
	}
}





static std::vector<OpenBuilding*> m_openlist;

std::vector<OpenBuilding*> OpenBuilding::getData()
{
	if (m_openlist.empty())
	{
		initData();
	}
	return m_openlist;
}

void OpenBuilding::initData()
{   

	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from c_openbuild");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());
	while (!query.eof())
	{
		OpenBuilding *item = new OpenBuilding();
		item->id = query.getIntField("id");
		item->type =  (BuyBuildingType)query.getIntField("type");
		item->openlevel = query.getIntField("openlevel");
		item->level1 = query.getIntField("level1");
		item->level2 = query.getIntField("level2");
		item->level3 = query.getIntField("level3");
		item->level4 = query.getIntField("level4");
		item->level5 = query.getIntField("level5");
		item->level6 = query.getIntField("level6");
		item->level7 = query.getIntField("level7");
		item->level8 = query.getIntField("level8");
		item->level9 = query.getIntField("level9");
		item->level10 = query.getIntField("level10");
		m_openlist.push_back(item);
		query.nextRow();
	}
	query.finalize();
}

int OpenBuilding::getLimitCountFormLevel( int level )
{   
	int resutl = 0;
	switch (level)
	{
	case 1:
		resutl = level1;
		break;
	case 2:
		resutl = level2;
		break;
	case 3:
		resutl = level3;
		break;
	case 4:
		resutl = level4;
		break;
	case 5:
		resutl = level5;
		break;
	case 6:
		resutl = level6;
		break;
	case 7:
		resutl = level7;
		break;
	case 8:
		resutl = level8;
		break;
	case 9:
		resutl = level9;
		break;
	case 10:
		resutl = level10;
		break;
	default:
		break;
	}
	return resutl;
}
