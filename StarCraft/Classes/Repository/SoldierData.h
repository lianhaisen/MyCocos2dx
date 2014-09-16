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
    int type;//����id ��70000101
	int camp;//��Ӫ
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

	//�Ƴ�ĳ�����ֵĵ�һ��ʿ��
	//static void removeSoldierByType(int type);
	static void removeSoldier(int type, int id);
};


#endif //__SOLDIERDATA_H__