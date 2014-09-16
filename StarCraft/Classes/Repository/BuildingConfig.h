#ifndef __BUILDINGCONFIG_H__
#define __BUILDINGCONFIG_H__
#include <string>
#include <vector>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

/*
//攻击目标
enum E_AttackTarget
{
	e_none,
	e_ground,
	e_air,
	e_all
};
//攻击类型
enum E_AttackType
{
	e_none,
	e_single,
	e_aoe
};
//攻击偏好
enum E_AttackPreference
{
	e_none,
};
*/


class CBuilding
{
public:
	enum class ResourceType
	{   
		//蓝水晶
		BLUE = 1,
		//黑水晶
		BLACK = 2,
		//金属
		GOLD = 3,
		//能量石
		STONE = 4,
		//钻石
		DIAMOND = 5
	};

public:
	int id;						//标识（类型+等级）
	string name;				//名称
	int type;					//建筑类型标识
	int grid;					//网格数量
	int level;					//等级
	int townhall;				//大本等级
	int cost_time;				//升级时间S
	ResourceType cost_type;		//建造消耗资源类型
	int cost_value;				//建造资源
	int max_hp;					//生命值
	int attack;					//攻击值
	//塔防类型
	int attack_target;			//攻击目标类型（对空，对地）
	int preference;				//攻击偏好
	int attack_type;			//攻击类型
	int aoe_range;				//AOE范围
	int min_range;				//最小攻击范围
	int max_range;				//最大攻击范围
	int attack_speed;			//攻击速度S
	int defense_points;			//防御积分
	//设施类型
	int sub_attack;				//降低伤害%
	int add_attack;				//自身伤害值%
	//陷阱类型
	int cost_repair;			//修理资源
	int trap_type;				//陷阱类型
	int trap_effect;			//陷阱效果
	int attack_range;			//伤害半径
	int trigger_range;			//触发半径
	int trap_delay;				//爆炸需时S
	int slow_time;				//缓慢时间S
	int slow_speed;				//速度降低%
	int stun_time;				//眩晕时间S
	int freeze_time;			//冰冻时间S
	int smoke_time;				//烟雾时间S
	int poison_time;			//中毒时间S
	int poison_attack;			//中毒伤害
	//建筑类型
	int alert_range;			//警戒范围
	int storage_type;			//存储类型
	int storage;				//存储容量
	int output;					//小时产量H
	int population;				//容纳人口
	//其他字段
	int race;					//种族
	int power_points;			//能量点
	int command;				//小岛统率
	int exp;					//经验值
	int building_type;			//建筑类型（塔防，资源建筑，普通建筑，设施）用于攻击偏好
	float fly_speed;			//弹道飞行速度
};


class BuildingConfig
{
public:
	BuildingConfig(void);
	~BuildingConfig(void);
public:
	static map<int, CBuilding*> getBuildingConfig();
	static CBuilding* getBuildingConfig(int id);
private:
	static void initBuildingConfig();

};


class OpenBuilding
{
public:
	enum class BuyBuildingType
	{   
		//基础建筑
		FONDATION = 1,
		//防御建筑
		DEFENSE,
		//资源建筑
		RESOURCE,
		//陷阱建筑
		TRAP,
	};

public:
	//类型
	int id;
	//建筑类型
	BuyBuildingType type;
	//开启等级
	int openlevel;
	//大本1
	int level1;
	int level2;
	int level3;
	int level4;
	int level5;
	int level6;
	int level7;
	int level8;
	int level9;
	int level10;
	int getLimitCountFormLevel(int level);
public:
	static std::vector<OpenBuilding*> getData();
	static void initData();
};

class BuyListConfig
{
public:
	CBuilding * buildcfg;
	int current;
	OpenBuilding * opencfg;
public: 
	BuyListConfig();
	~BuyListConfig(){}
    BuyListConfig(CBuilding* scfg);
    static std::vector<BuyListConfig*> getBuyListFromType(OpenBuilding::BuyBuildingType type = OpenBuilding::BuyBuildingType::FONDATION);
	static BuyListConfig * getBuyListFromId(int id);

private:
    static void initBuyList();
};







#endif //__BUILDINGCONFIG_H__