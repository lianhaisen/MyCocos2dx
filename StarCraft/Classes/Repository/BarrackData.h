#ifndef  _BARRACKDATA_H
#define  _BARRACKDATA_H

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

#include "SoldierConfig.h"

//TODO 这里的列表都是数字加逗号隔开

//兵种对象
class BarrackItem
{
public:
	int id;
	int soldierid;
	int number;
	int population;
	//士兵可使用数量
	int soldierstate;
};
class BarrackData
{
public:
	//建筑物ID
	int id;
	//最大人口
	int maxpopulation;
	//当前人口
	int curpopulation;
	//士兵列表
	list<BarrackItem*> soldieritems;

	void addSoldier(int id);

	void updateData();

	void updataSoldierUseState(int Soldierid,int Barrackid,int nNum);


private:
	//士兵类型列表
	string soldiertypeids;
	//士兵数目列表,一一对应
	string soldiernums;
	//每个兵所占人口
	string soldiersharepopulations;
	//士兵使用状态
	string soldierstates;
public:
	static map<int,BarrackData*> getBarrackDataMap();
	static BarrackData * getBarrackFromId(int id);
	static BarrackData * createNew(int id);

private:
	static void initBarrackData();

};

class TrainItem
{
public:
	int soldierid;
	int number;
	int trainingtime;
	int population;
};

class TrainingData
{
public:
	//建筑物ID
	int id;
	//最大人口
	int maxpopulation;
	//当前人口
	int curpopulation;
	//士兵列表
	list<TrainItem *> soldieritems;
	//当前这个剩余时间
	int curtraininglavetime;
	//获得下一个需要的时间
	int getNextCanBuildTime();

	void deletaitems(); 

	//获得总共时间
	int getTotalTime();
	//更新数据
	void updateData();
private:
	//士兵类型列表
	string soldiertypeids;
	//士兵数量列表
	string soldiernums;
	//士兵训练时间
	string soldiertrainingtimes;
	//士兵占人口列表
	string soldiersharepopulations;
public:
    static map<int,TrainingData*> getTrainingDataMap();
	static TrainingData * getTrainingDataFromId(int id);
	static TrainingData *CreateItem(int id);
private:
	static void initTrainingData();

};

class SoldierUpgradeData
{
public:
	SoldierUpgradeData();
	~SoldierUpgradeData();
public:
	//建筑物ID
	int id;
	//兵种类型
	int type;
	//升级状态 0 正常 1升级中
	int status;
	//升级完成时间点
	long long timestamp;
	UserSoldierLevel * getSoldierLevel();
    int getLeftTime();
private:
	UserSoldierLevel * _userLevel;

public:
	static map<int,SoldierUpgradeData*> getSoldierUpgradeMap();
	static SoldierUpgradeData* getSoldierUpgradeMapFromId(int id);
	static void UpgradeSoldier(int bid,int soldiertype,int time);
private:
	static void initSoldierUpgradeData();

};

//运输机信息
class ConveyorConfig
{
public:
	//运输机ID
	int id;
	//总数量
	int soldierAllNum;
	//士兵信息
	list<BarrackItem*> soldieritems;

	//增加
	void addSoldier(int barrackId,int soldierid); 
	//减少
	void DeleteSoldier(int barrackId,int soldierid );
private:
	//军营ID
	string barrackIds;
	//士兵类型列表
	string soldiertypeids;
	//士兵数量列表
	string soldiernums;
public:
	static map<int,ConveyorConfig*> getConveyorConfig();
	static ConveyorConfig* getConveyorConfigFromId(int id);
	static void CreateItem(int id);

private:
	static void initConveyorConfig();
	//更新数据
	void updateData();
};

#endif



