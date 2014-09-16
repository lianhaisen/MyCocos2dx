#ifndef __BUILDINGFUNC_H
#define __BUILDINGFUNC_H
using namespace std;
#include "BuildingData.h"
#include "UIUserMinUi.h"
#include "SoldierConfig.h"
#include "BarrackData.h"

//收资源等逻辑

// 将要去建的士兵
class SolidToBuild
{
public:
	//士兵ID
	int solidId;
	//数目
	int num;
	//当前经过时间
	int curpass;
};

class BuildingFunc
{
private:
	BuildingFunc();
	~BuildingFunc();
public:
	static BuildingFunc * getInstance();
	//统计建筑
	void addContains(DBuilding * db);
	//收集资源
	void CollectResource(DBuilding * db);
	//获得主建筑物等级
	int getMainLevel();
	//获得科学院等级
	int getScienceLevel();
	void setMinUI(UIUserMinUi* ui);
	//获得
	vector<CSoldier*> getSolidListFromDBuilding(DBuilding * db);
	//获得所有士兵
	map<int,BarrackItem*> getAllSoldier();

	//增加士兵
	void addSoldier(Coord pos,int id,int popution);

	vector<DBuilding*> getBarracks(){return _barracks;}
	DBuilding* getBarrackFromIndex(int i);
	int        getBarrackIndexFormDB(DBuilding * db);


private:
	int getLeftCanStore(vector<DBuilding*> ve);
	vector<DBuilding*> getContainsFromType(int type);
	//得到最大能容纳的
	int getMaxCanStore(vector<DBuilding*> ve);
	//得到当前的容量
	int getCurStroe(vector<DBuilding*>ve);

	void AddResourceToMainUI(int type,int count);
 
private:
	//蓝水晶仓库
	vector<DBuilding*> _blueContains;
	//金属仓库
	vector<DBuilding*> _goldContains;
	//黑水晶仓库
	vector<DBuilding*> _blackContains;
	//主基地
	DBuilding*         _mainBuilding;
    
	map<int,BarrackItem*> _allSoldiers;
	//军营
	vector<DBuilding*> _camps;

	//兵营(产兵的地方)
	vector<DBuilding*> _barracks;
	
	//研究院
	DBuilding * _science;

	//兵营可建造队列
	map<DBuilding*,vector<CSoldier*>> _barracksBuildlist;

};



#endif



