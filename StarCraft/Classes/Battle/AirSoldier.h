#ifndef __AIRSOLDIER_H__
#define __AIRSOLDIER_H__
/*
*实现飞机场生产的士兵，包括大鸟直升机、幽灵战机、运输机、黑蝙蝠战机、铁鹰战机、战列舰
*/
#include "Soldier.h"


//大鸟直升机700011
class SoldierDnzsj : public Soldier
{
public:
	SoldierDnzsj(int id);
	~SoldierDnzsj(void);

	virtual void runWalkAction();//走路动作
};

//幽灵战机700012
class SoldierYlzj : public Soldier
{
public:
	SoldierYlzj(int id);
	~SoldierYlzj(void);

	virtual void runWalkAction();//走路动作
};

//运输机700013
class SoldierYsj : public Soldier
{
public:
	SoldierYsj(int id);
	~SoldierYsj(void);

	virtual void runWalkAction();//走路动作
};

//黑蝙蝠战机700014
class SoldierHbfzj : public Soldier
{
public:
	SoldierHbfzj(int id);
	~SoldierHbfzj(void);

	virtual void runWalkAction();//走路动作
};

//铁鹰战机700015
class SoldierTyzj : public Soldier
{
public:
	SoldierTyzj(int id);
	~SoldierTyzj(void);

	virtual void runWalkAction();//走路动作
};

//战列舰700016
class SoldierZlj : public Soldier
{
public:
	SoldierZlj(int id);
	~SoldierZlj(void);

	virtual void runWalkAction();//走路动作
};
#endif // __AIRSOLDIER_H__
