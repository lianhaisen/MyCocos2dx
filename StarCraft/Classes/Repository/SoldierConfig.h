#ifndef __SOLDIERCONFIG_H__
#define __SOLDIERCONFIG_H__
#include <string>
#include <vector>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;


class CSoldier
{
public:
	int id;						//标识（类型+等级）
	string name;				//名称
	int type;					//类型标识
	//int level;				//等级
	int attack_type;			//攻击类型（单体，AOE）
	int preference;				//攻击偏好
	int attack_target;			//攻击目标类型（对空，对地）
	float aoe_range;			//AOE范围
	int min_range;				//最小攻击范围
	int max_range;				//最大攻击范围
	float attack_speed;			//攻击速度S
	int attack;					//攻击值
	int max_hp;						//生命值
	int dot;					//持续伤害
	int dot_time;				//持续伤害时间S
	float move_speed;			//移动速度
	int population;				//占人口
	int air_population;			//空运人口
	int train_time;				//训练时间S
	int is_air;					//空运
	int is_invisible;			//隐形
	int is_jump;				//跳跃
	int camp_level;				//兵营等级
	int lab_level;				//科学实验室等级
	int building_id;			//生产建筑ID	
	int skill_type;				//技能类型ID
	int cost_type;				//消耗资源类型
	int train_cost;				//生产消耗资源数量
	int upgrade_cost;			//升级消耗资源数量
	int upgrade_time;			//升级时间S
	int race;					//种族
	float power_points;			//能量点
	int chip;					//碎片数量	
	int unit_type;				//单位类型（地面，空中）
	int life_type;				//生命类型（生命，机械）
	int soldier_points;			//兵种积分
	float fly_speed;			//弹道飞行速度
};


class SoldierConfig
{
public:
	SoldierConfig(void);
	~SoldierConfig(void);
public:
	static map<int, CSoldier*> getSoldierConfig();
	static CSoldier* getSoldierConfig(int id);
private:
	static void initSoldierConfig();

};

//用户当前兵种升级情况
class UserSoldierLevel
{
public:
	UserSoldierLevel();
	~UserSoldierLevel(){}
public:
	//类型
	int id;
	//等级
	int level;
    
	//更新等级
	void updateLevel();

	CSoldier * getCurcfg();
	CSoldier * getNextcfg();
private:
	CSoldier * _curcfg;
	CSoldier * _nextcfg;
public:
	static map<int,UserSoldierLevel*> getUserSoldierLevels();
	static UserSoldierLevel * getUserSoldierLevelFromId(int id);
private:
	static void initSoldierLevels();

};



#endif //__SOLDIERCONFIG_H__