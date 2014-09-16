#include "BuildingObserver.h"
#include "BattleData.h"

static BuildingObserver* m_Instance;
BuildingObserver* BuildingObserver::getInstance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new BuildingObserver();
	}
	return m_Instance;
}


void BuildingObserver::addObserver( int grid, int buildingId )
{
	map<int, vector<int>>::iterator it = observers.find(grid);
	if (it == observers.end())
	{
		vector<int> bs;
		bs.push_back(buildingId);
		observers.insert(map<int, vector<int>>::value_type(grid, bs));
	} 
	else
	{
		vector<int> bs = observers.at(grid);
		bs.push_back(buildingId);
	}
}

void BuildingObserver::deleteObserver( BattleLogic* building )
{



}

void BuildingObserver::notifyObserver(int grid, int soldierId)
{

}

void BuildingObserver::release()
{
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}

bool BuildingObserver::check( int grid, int soldierId )
{
	log("BuildingObserver::check");
	map<int, vector<int>>::iterator it1 = observers.find(grid);
	if (it1 != observers.end())
	{
		vector<int> bs = it1->second;
		vector<int>::iterator it2;
		for(it2=bs.begin(); it2!=bs.end(); it2++)
		{
			BattleLogic *building = BattleData::getInstance()->getDefendBuilding(*it2);
			//building->addCandidateTarget(soldierId);
			//building->setAttackDirection();
			//building->attackTarget(); 
		}
		return true;
	} 
	return false;
}
