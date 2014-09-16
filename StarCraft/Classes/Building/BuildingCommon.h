#ifndef _BUILDINGCOMMON_H_
#define _BUILDINGCOMMON_H_

#include "cocos2d.h"
using namespace cocos2d;
#include "BuildingData.h"
#include "UIPanZoomLayer.h"
#include "BattleLogic.h"
#include "AppMacros.h"

class BuildingCommon
{
public: 

	//************************************
	// Method:    getFilePathFromId 根据ID获得文件名
	// FullName:  BuildingCommon::getFilePathFromId
	// Access:    public static 
	// Returns:   std::string
	// Qualifier:
	// Parameter: int id
	//************************************
	static std::string getFilePathFromId(int id); 

	//获得动作对象
	static std::string getStringFromLevelAndStatus(int level,DBuilding::BuildingStatus status);
	static std::string getNomalTowerStringFromLevel(int level,int degree = 1);
	static std::string getTowerStringFromLevel(int level,int degree = 1);
	static std::string getTowerBodyStringFromLevel(int level);
	
	//攻击串
	static std::string getAttackstring(int level);
    //飞行道具
	static std::string getFlystring(int level);
	//获得爆炸效果
	static std::string getBoomstring(int level);

	static std::string getFlyTowerstring(int level,int degree);

	//加载动画资源
	static void LoadArmateResource();

	static std::string getArmatureNameFromType(int type);

	static std::string getGrassNameFromGridCount(int count);

	static std::string getPicFromId(int id);

	static Point getTmpPoint();
	static Point getTmpPoint2();
    static void  setTmpPoint(Point p1,Point p2);

	static std::string getHeroPicFromId(int id);

	static std::string getWallStr(int level,int state);

	//获得界面layer
	//setDelegate 获得点击点数据
	//addBuilding 添加建筑
	//getPlaceLayer h获得摆放层
	//参数
	//isInbattle 是否在战斗中,战斗中建筑物不可移动
	//buildingselect 建筑物点击回调
	//db 传入构建建筑物的数据
	static UIPanZoomLayer * getPlayer(bool isInbattle = false,const std::vector<DBuilding*>* db = nullptr,const ccMenuCallback& buildingselect = nullptr);

	//static UIPanZoomLayer * getPlayer2(std::vector<BBuilding*> database,std::map<int,BattleLogic*> & mapOut);

	static UIPanZoomLayer * getPlayer3(const std::vector<DBuilding*> db,std::vector<BattleLogic*> & mapOut);

	static Layer * getMainMap();


};


#endif



