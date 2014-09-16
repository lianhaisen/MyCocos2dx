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

	//void setAttackSoldierInBattle(map<int, Soldier*> &soldiers);//TODO:ò�Ʋ���Ҫ
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
	map<int, BattleLogic*> _attacker;//��������ʿ����Ӣ�ۣ�
	map<int, BattleLogic*> _defender;//���ط���������ʿ����Ӣ�ۣ�
	
	map<int, vector<DSoldier*>> _attSoldierInCamp;//����������Ӫ�е�ʿ���������ַ���
	map<int, BattleLogic*> _attSoldierInBattle;//�����������ϳ���ʿ��
	map<int, BattleLogic*> _defBuilding;//���ط�����

	map<int, NewCoord*> _newCoord;//ʿ������map
};

#endif // __BATTLEDATA_H__