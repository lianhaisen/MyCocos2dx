#ifndef __BACEOBJECT_H__
#define __BACEOBJECT_H__
#include "MapHelper.h"

#define E_STATUS_DEAD 99

//战斗状态
enum E_BattleStatus
{
	e_battle_status_idle = 0,				//空闲
	e_battle_status_relax = 1,				//休闲
	e_battle_status_path = 2,				//开始寻路
	e_battle_status_pathing = 3,			//寻路中（查找攻击目标，并寻路，并未开始移动）
	e_battle_status_move = 4,				//开始移动
	e_battle_status_moveing = 5,			//移动中
	e_battle_status_attack = 6,				//开始攻击
	e_battle_status_attacking = 7,			//攻击中
	e_battle_status_dead = E_STATUS_DEAD,	//死亡
	e_battle_status_deaded = 100			//已死亡
};

//方向
enum E_BattleDir
{
	e_battle_dir_none = 0,		//无
	e_battle_dir_east = 1,		//东
	e_battle_dir_es = 2,		//东南
	e_battle_dir_south = 3,		//南
	e_battle_dir_ws = 4,		//西南
	e_battle_dir_west = 5,		//西
	e_battle_dir_wn = 6,		//西北
	e_battle_dir_north = 7,		//北
	e_battle_dir_en = 8,		//东北
};

//攻击目标类型（对空，对地……）
enum E_Attack_Target
{
	e_attack_target_none,//不能攻击
	e_attack_target_air,//对空
	e_attack_target_ground,//对地
	e_attack_target_all,//对空对地
	e_attack_target_ground2//对地高射
};

enum E_Attack_Preference
{
	e_preference_none,//无
	e_preference_life,//生命单位
	e_preference_machine,//机械单位
	e_preference_tower,//塔防
	e_preference_resource,//资源建筑
	e_preference_normal,//普通建筑
	e_preference_facility,//设施
	e_preference_all1,//全部建筑(普通建筑+资源建筑)
	e_preference_all2,//全部建筑及设施
	e_preference_all3,//全部建筑及塔防
	e_preference_trap,//陷阱
	e_preference_barrier1,//可移除障碍
	e_preference_barrier2//不可移除障碍
};

enum E_Unit_Type
{
	e_unit_type_none,//无
	e_unit_type_ground,//地面
	e_unit_type_air,//空中

};

class BattleLogic;
struct NewCoord
{
	int key;//x*MAX_GRID_COUNT+y
	//BattleLogic* soldier;
	vector<BattleLogic*> soldierlist;
};

class BattleLogic
{
public:
	BattleLogic(void);
	~BattleLogic(void);
/**************************************************
通用方法
**************************************************/
	virtual int getId();
	virtual void setId(int id);
	virtual int getType();
	virtual void setType(int type);
	virtual E_BattleStatus getStatus();
	virtual void setStatus(E_BattleStatus status);
	virtual Coord getCoord();//返回中心点坐标
	virtual void setCoord(Coord coord);
	virtual int getHp();//获取当前hp
	virtual void setHp(int hp);//初始化hp和maxHP
	//扣血，返回是否死亡，护士加血，返回血是否加满
	virtual bool subHp(int hp);
	virtual bool isFullHp();//是否满血
	virtual void setTarget(BattleLogic* target);//设置目标
	virtual BattleLogic* getTarget();//获取目标
    
	//获得新坐标 有需要被炮塔攻击的才要重载
	virtual NewCoord* getNewCoord(){return nullptr;}
	//执行动作 
	virtual void  startLogic(){} 
	

	virtual void updateHpProgress(){};//更新血条百分比
	virtual void dead(){};

	//设置和活动中心点
	virtual void  setMidCoord(Coord coord){_midcoord = coord;}
	virtual Coord getMidCoord(){return  _coord;}

protected:
	unsigned int _id;//每个士兵的唯一标识
	unsigned int _typeid;
	E_BattleStatus _status;
	Coord _coord;
	Coord _midcoord;
	int _maxHp;//最大hp
	int _hp;//当前hp
	BattleLogic *_target;//目标 TODO:攻击目标还有可能是敌方士兵，英雄

};






#endif // __BACEOBJECT_H__