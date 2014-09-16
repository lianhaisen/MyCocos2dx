#include "BattleData.h"
#include "AppMacros.h"


BattleData *s_Instance = nullptr;

BattleData::BattleData( void )
{

}

BattleData::~BattleData( void )
{

}

BattleData* BattleData::getInstance()
{
	if (s_Instance == nullptr)
	{
		s_Instance = new BattleData();
	}
	return s_Instance;
}

void BattleData::release()
{
	//map<int, BattleLogic*>::iterator it1;
	//for (it1 = s_Instance->_attacker.begin(); it1 != s_Instance->_attacker.end(); it1++)
	//{
	//	delete(it1->second);
	//}
	//s_Instance->_attacker.clear();

	//map<int, BattleLogic*>::iterator it2;
	//for (it2 = s_Instance->_defender.begin(); it2 != s_Instance->_defender.end(); it2++)
	//{
	//	delete(it2->second);
	//}
	//s_Instance->_defender.clear();

	
	//TODO:施放内存，根据实际情况，清空容器内部申请的内存
	CC_SAFE_DELETE(s_Instance);

}

map<int, BattleLogic*> BattleData::getAttacker()
{
	return _attacker;
}

map<int, BattleLogic*> BattleData::getDefender()
{
	return _defender;
}

void BattleData::setAttackSoldierInCamp( map<int, vector<DSoldier*>> &soldiers )
{
	_attSoldierInCamp = soldiers;
}

void BattleData::deleteSoldierInCamp( BattleLogic* soldier )
{
	map<int, vector<DSoldier*>>::iterator it1 = _attSoldierInCamp.find(soldier->getType());
	if (it1 != _attSoldierInCamp.end())
	{
		//vector<DSoldier *> soldiers = it1->second;
		vector<DSoldier *>::iterator it;
		for (it = it1->second.begin(); it < it1->second.end(); it++)
		{
			auto item = *it;
			if (item->id == soldier->getId())
			{
				//TODO:作战结算时，没有死亡的士兵回到兵营
				SoldierData::removeSoldier(item->type, soldier->getId());
				it1->second.erase(it);
				break;
			}
		}
	}
}

map<int, vector<DSoldier*>> BattleData::getAttackSoldierInCamp()
{
	return _attSoldierInCamp;
}

//void BattleData::setAttackSoldierInBattle( map<int, Soldier*> &soldiers )
//{
//	_attSoldierInBattle = soldiers;
//}

void BattleData::addAttackSoldierInBattle( BattleLogic* soldier )
{
	_attSoldierInBattle.insert(map<int, BattleLogic *>::value_type(soldier->getId(), soldier));
}

void BattleData::setDefendBuilding( map<int, BattleLogic*> &buildings )
{
	_defBuilding = buildings;
}

map<int, BattleLogic*> BattleData::getAttackSoldierInBattle()
{
	return _attSoldierInBattle;
}

map<int, BattleLogic*> BattleData::getDefendBuilding()
{
	return _defBuilding;
}

BattleLogic* BattleData::getDefendBuilding( int id )
{
	for (auto item : _defBuilding)
	{
		if (item.first == id)
		{
			return item.second;
		}
	}
	return nullptr;
}

NewCoord* BattleData::getSoldierCoord( Coord coord )
{   
	NewCoord * item = nullptr;
	int H = coord.x * MAX_GRID_COUNT + coord.y;
	if (_newCoord.empty())
	{
		item = new NewCoord();
		item->key = H;
		_newCoord[item->key] = item;
	}
	else
	{
		auto result = _newCoord.find(H);
		if (result != _newCoord.end())
		{
			item = result->second;
		}
		else
		{
			item = new NewCoord();
			item->key = H;
			_newCoord[item->key] = item;
		}
	}
	return item;
}










