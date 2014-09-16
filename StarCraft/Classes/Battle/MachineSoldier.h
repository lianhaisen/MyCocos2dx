#ifndef __MACHINESOLDIER_H__
#define __MACHINESOLDIER_H__
/*
*实现机械工厂生产的士兵，包括机械战警、烈焰战车、攻城车、坦克、雷神机器人
*/
#include "Soldier.h"


//机械战警700006
class SoldierJxzj : public Soldier
{
public:
	SoldierJxzj(int id);
	~SoldierJxzj(void);

};

//烈焰战车700007
class SoldierLyzc : public Soldier
{
public:
	SoldierLyzc(int id);
	~SoldierLyzc(void);

	virtual void runWalkAction();//走路动作
	virtual void doMove();//向目标移动
	virtual void doAttack();//攻击目标，护士是为目标加血


private:
	CC_SYNTHESIZE_RETAIN(Armature*,_head,Head);
	double getBeginDegree();//初始化当前角度
	double getEndDegree();
	void rotate();

	int getIndexFormDegree(int degree);
	string getMovement(int from, int to);
	std::string getTowerStringFromLevel(int level, int degree = 1);
	void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);
private:
	//double _currentDegree;//当前角度
	//double _attackDegree;//攻击角度
	
	int _degreeIndex;
	int _currentIndex;
};

//攻城车700008
class SoldierGcc : public Soldier
{
public:
	SoldierGcc(int id);
	~SoldierGcc(void);

	virtual void runWalkAction();//走路动作
};

//坦克700009
class SoldierTk : public Soldier
{
public:
	SoldierTk(int id);
	~SoldierTk(void);

	virtual void runWalkAction();//走路动作

private:
	CC_SYNTHESIZE_RETAIN(Armature*,_head,Head);
};

//雷神机器人700010
class SoldierLsjqr : public Soldier
{
public:
	SoldierLsjqr(int id);
	~SoldierLsjqr(void);

};
#endif // __MACHINESOLDIER_H__
