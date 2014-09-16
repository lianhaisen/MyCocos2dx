#ifndef __BUILDINGDATA_H__
#define __BUILDINGDATA_H__
#include <string>
#include <vector>
#include "cocos2d.h"
#include "MapHelper.h"


using namespace std;
using namespace cocos2d;

class CBuilding;
class Building;


class DBuilding
{
public:
	enum class BuildingStatus
	{   
		//空闲
		RELEX  = 0,
		//升级中
		UPGRADE = 1,
		//生产
		WORK = 2,
		//新建
		NewBuild = 3
	};
public:
	DBuilding();
	~DBuilding();

public:
	int id;
    int type; //类型
	Coord coord;//网格坐标
	BuildingStatus status;
	long long timestamp;//时间戳
	int  curpro; //当前产量
public:
	
	//************************************
	// Method:    updateData 更新数据
	// Parameter: int type 类型ID
	void updateCoor(int x,int y);
	//设置升级
	bool setUpgrade();
	//获得剩余时间(建筑或者新建)
	int getLeftTime(); 

	//更新产量
	void updatePro();

    
	//获得当前等级的绑定数据
    CBuilding * getCurBindCfg();
	//获得下一等级的绑定数据
	CBuilding * getNextBindCfg();
	//获得最大等级的绑定数据
	CBuilding * getMaxBindCfg();

	//获得建筑物
	Building * getBuilding(){return _building;}
	//设置建筑物
	void setBuilding(Building * building){_building = building;}

private:
	CBuilding * _curCfg;
	CBuilding * _nextCfg;
	CBuilding * _maxCfg;
	Building * _building;
};

class BuildingData
{
public:
	BuildingData(void);
	~BuildingData(void);
public:
	static vector<DBuilding*> getBuildingData();
	static DBuilding* getBuildingData(int id);

	//************************************
	// Method:    创建新建筑数据
	// FullName:  BuildingData::CreateDBuilding
	// Access:    public static 
	// Returns:   DBuilding*
	// Qualifier:
	// Parameter: int type 建筑类型id
	// Parameter: int x    
	// Parameter: int y
	//************************************
	static DBuilding* CreateDBuilding(int type,int x,int y);
	


private:
	static void initBuildingData();
};


#endif //__BUILDINGDATA_H__