#ifndef __SOLDIERDATA_H__
#define __SOLDIERDATA_H__
#include <string>
#include <vector>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class DSoldier
{
public:
	int id;
    int type;//兵种id 如70000101
	int camp;//兵营
};

class SoldierData
{
public:
	static void initSoldierData();
	static map<int, DSoldier *> getSoldierData();
	static DSoldier* getSoldierData(int id);
	static vector<DSoldier *> getSoldierDataByType(int type);
	static map<int, vector<DSoldier *>> getSoldierDataByType();
	static vector<DSoldier *> getSoldierDataByCamp(int camp);
	static map<int, vector<DSoldier *>> getSoldierDataByCamp();

	//移除某个兵种的第一个士兵
	//static void removeSoldierByType(int type);
	static void removeSoldier(int type, int id);
};


#endif //__SOLDIERDATA_H__