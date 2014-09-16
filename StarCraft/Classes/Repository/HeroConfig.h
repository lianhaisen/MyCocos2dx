#ifndef  _HEROCONFIG_H
#define  _HEROCONFIG_H

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
//英雄信息
class HeroInfo
{
public:
	//英雄类型
	int id;
	//种族
	int race;
	//名称
	std::string name;
	//攻击类型
	int attacktype;
	//单位类型
	int unittype;
	//攻击偏好
	int attackpre;
	//伤害类型
	int hurttype;
	//AOE范围
	int AOErange;
	//连发次数
	int burstscount;
	//最小攻击
	int attackmin;
	//最大攻击
	int attackmax;
	//攻击速度
	int attackspeed;
	//移动速度
	int movespeed;
	//警戒半径
	int alertradius;
	//所占人口
	int population;
	//能量点
	int energypoint;
	//主本等级
	int mainlv;
	//能量石
	int energystone;
	//技能ID
	int skillid;
	//技能名称
	std::string skillname;
	//弹道飞行时间
	int bulletflytime;
public:
	static map<int,HeroInfo*> getHeroConfig();
	static HeroInfo* getHeroFromId(int id);
private:
	static void initHeroConfig();

};

//英雄基础升级信息
class HeroBaseUpgrade
{
public:
	//等级
	int id;
	//消耗类型
	int costtype;
	//消耗的东西
	int cost;
	//消耗时间
	int costtiem_m;
	//英雄重生时间
	int rebirthtime_m;
	//荣誉值
	int honor;
public: 
	static map<int,HeroBaseUpgrade*> getHeroBaseUpgrade();
	static HeroBaseUpgrade* getHeroBaseFromId(int id);
private:
	static void initHeroBaseUpgrade();
};

//英雄升级
class HeroUpgrade
{
public:
	HeroUpgrade();
	~HeroUpgrade();
public:
	//英雄ID
	int id;
	//类型
	int type;
	//名称
	std::string name;
	//伤害
	int hurt;
	//血量
	int hp;
    //获得基本信息
	HeroBaseUpgrade * getBaseInfo();
private:
	HeroBaseUpgrade * _baseinfo;

public:
	static map<int,HeroUpgrade*> getHeroUpragrage();
	static HeroUpgrade* getHeroUpgrageFromId(int id);
private:
	static void initHeroUpragrade();
};

//英雄技能升级
class HeroSkillUpgrade
{
public:
	//技能ID
	int id;
	//技能类型
	int type;
	//技能名称
	std::string name;
	//消耗类型
	int costtype;
    //消耗
	int cost;
	//消耗时间
	int costtime;
    //效果时间
	int effecttime;
    //攻击增幅
	int attackadd;
	//移动增幅
	int moveadd;
	//爆炸伤害
	int blowhurt;
	//变身减轻伤害
	int turnedloseinjury;
	//每下治疗
	int treat;
	//治疗次数
	int treatcount;
	//治疗间隔
	int treatinterval;
	//降低攻速
	int reduceattackspeed;

public:
	static map<int,HeroSkillUpgrade*> getHeroSkillUpgrade();
	static HeroSkillUpgrade* getHeroSkillFromId(int id);
private:
	static void initHeroSkillUpgrade();
};

//一种英雄只能招募一个
class HeroLevelAndSkillLevel
{
public:
	HeroLevelAndSkillLevel();
public:
	//英雄类型ID
	int id; 
	//英雄等级
	int herolevel;
	//技能等级
	int skilllevel;
	//状态
	int status;
    //0 英雄 1技能
	int isupgradehero;
	//时间戳
	long long timestamp;

private:
	//等级转换
	int level;
	void getRealLevel();
	void getToLevel();
public:
	static map<int,HeroLevelAndSkillLevel*> getHeroLevelAndSkillLevel();
	static HeroLevelAndSkillLevel* getHeroLevelAndSkillLevelFromId(int id);
	static void createHeroItem(int nType,int nLevel = 1);
	int getTotalTime();
	bool setUpData(int nUpgradeHero);
private:
	static void init();
};

//   英雄祭坛配置信息
class HeroAltarConfig
{
public:
	HeroAltarConfig();
	void UpdataHeroToAltar(int nType);
public:
	//英雄祭坛ID
	int id;
	//英雄类型ID
	int type;
public:
	static map<int,HeroAltarConfig*> getHeroAltarConfig();
	static HeroAltarConfig* getHeroAltarConfigFromId(int id);
	static void createHeroToAltar(int nId,int nType);

private:
	static void init();

};


#endif



