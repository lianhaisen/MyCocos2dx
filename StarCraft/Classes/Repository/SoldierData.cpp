#include "SoldierData.h"
#include "SqliteHelper.h"


static map<int, DSoldier *> s_SoldierData;
static map<int, vector<DSoldier *>> s_SoldierDataByType;
static map<int, vector<DSoldier *>> s_SoldierDataByCamp;

void SoldierData::initSoldierData()
{
	s_SoldierData.clear();
	s_SoldierDataByType.clear();
	s_SoldierDataByCamp.clear();

	TSQLite3DB *sqliteHandler = SqliteHelper::getSqliteHandler();
	std::string strSQL = std::string("select * from d_soldier");
	TSQLite3Query query = sqliteHandler->execQuery(strSQL.c_str());

	while (!query.eof())
	{
		DSoldier *item = new DSoldier();
		item->id = query.getIntField("id");
		item->type = query.getIntField("type");
		item->camp = query.getIntField("camp");
		s_SoldierData[item->id] = item;

		map<int, vector<DSoldier *>>::iterator itType = s_SoldierDataByType.find(item->type);
		if (itType == s_SoldierDataByType.end())
		{
			vector<DSoldier *> types;
			types.push_back(item);
			s_SoldierDataByType.insert(map<int, vector<DSoldier *>>::value_type(item->type, types));
		} 
		else
		{
			itType->second.push_back(item);
		}

		map<int, vector<DSoldier *>>::iterator itCamp = s_SoldierDataByCamp.find(item->camp);
		if (itCamp == s_SoldierDataByCamp.end())
		{
			vector<DSoldier *> camps;
			camps.push_back(item);
			s_SoldierDataByCamp.insert(map<int, vector<DSoldier *>>::value_type(item->camp, camps));
		} 
		else
		{
			itCamp->second.push_back(item);
		}


		query.nextRow();
	}
	query.finalize();
}

map<int, DSoldier *> SoldierData::getSoldierData()
{
	if (s_SoldierData.empty())
	{
		initSoldierData();
	}

	return s_SoldierData;
}

DSoldier* SoldierData::getSoldierData(int id)
{
	if (s_SoldierData.empty())
	{
		initSoldierData();
	}
	DSoldier *item = s_SoldierData.at(id);
	return item;
}

vector<DSoldier *> SoldierData::getSoldierDataByType(int type)
{
	if (s_SoldierDataByType.empty())
	{
		initSoldierData();
	}
	return s_SoldierDataByType.at(type);
}

map<int, vector<DSoldier *>> SoldierData::getSoldierDataByType()
{
	if (s_SoldierDataByType.empty())
	{
		initSoldierData();
	}
	return s_SoldierDataByType;
}

vector<DSoldier *> SoldierData::getSoldierDataByCamp(int camp)
{
	if (s_SoldierDataByCamp.empty())
	{
		initSoldierData();
	}
	return s_SoldierDataByCamp.at(camp);
}

map<int, vector<DSoldier *>> SoldierData::getSoldierDataByCamp()
{
	if (s_SoldierDataByCamp.empty())
	{
		initSoldierData();
	}
	return s_SoldierDataByCamp;
}

//void SoldierData::removeSoldierByType( int type )
//{
//	map<int, vector<DSoldier *>>::iterator itType = s_SoldierDataByType.find(type);
//	if (itType != s_SoldierDataByType.end())
//	{
//		//itType->second.pop_front();
//	} 
//}

void SoldierData::removeSoldier( int type, int id )
{
	map<int, vector<DSoldier *>>::iterator itType = s_SoldierDataByType.find(type);
	if (itType != s_SoldierDataByType.end())
	{
		vector<DSoldier *> soldiers = itType->second;
		vector<DSoldier *>::iterator it;
		for (it = soldiers.begin(); it < soldiers.end(); it++)
		{
			auto item = *it;
			if (item->id == id)
			{
				soldiers.erase(it);
				break;
			}
		}
	} 
}


