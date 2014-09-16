#ifndef __BATTLEDATA_H__
#define __BATTLEDATA_H__
#include <vector>
#include "BattleLogic.h"
#include "Soldier.h"
#include "SoldierData.h"
#include "UIBuildingBase.h"

using namespace std;

class BattleData
{
public:
	BattleData(void);
	~BattleData(void);

	static BattleData* getInstance();
	static void release();

	map<int, BattleLogic*> getAttacker();
	map<int, BattleLogic*> getDefender();

	void setAttackSoldierInCamp(map<int, vector<DSoldier*>> &soldiers);
	map<int, vector<DSoldier*>> getAttackSoldierInCamp();
	void deleteSoldierInCamp(BattleLogic* soldier);

	//void setAttackSoldierInBattle(map<int, Soldier*> &soldiers);//TODO:貌似不需要
	void addAttackSoldierInBattle(BattleLogic* soldier);
	map<int, BattleLogic*> getAttackSoldierInBattle();


	void setDefendBuilding(map<int, BattleLogic*> &buildings);
	map<int, BattleLogic*> getDefendBuilding();
	BattleLogic* getDefendBuilding(int id);
	

	//void addSoldierCoord(Coord coord, BattleLogic* soldier);
	//void deleteSoldierCoord(Coord coord, BattleLogic* soldier);
	//void initCoodMap();
	//vector<NewCoord*> getCoodMap();
	NewCoord* getSoldierCoord(Coord coord);

private:
	map<int, BattleLogic*> _attacker;//进攻方（士兵，英雄）
	map<int, BattleLogic*> _defender;//防守方（建筑，士兵，英雄）
	
	map<int, vector<DSoldier*>> _attSoldierInCamp;//进攻方，兵营中的士兵，按兵种分类
	map<int, BattleLogic*> _attSoldierInBattle;//进攻方，已上场的士兵
	map<int, BattleLogic*> _defBuilding;//防守方建筑

	map<int, NewCoord*> _newCoord;//士兵坐标map
};

#endif // __BATTLEDATA_H__