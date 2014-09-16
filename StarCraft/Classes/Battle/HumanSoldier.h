#ifndef __HUMANSOLDIER_H__
#define __HUMANSOLDIER_H__
/*
*实现兵工厂生产的士兵，包括机枪兵、炮兵、护士、自杀机器人、火枪兵
*/
#include "Soldier.h"


//机枪兵700001
class SoldierJqb : public Soldier
{
public:
	SoldierJqb(int id);
	~SoldierJqb(void);
	//CC_SYNTHESIZE_RETAIN(Armature*,_fly,Fly);
	//CC_SYNTHESIZE_RETAIN(Armature*,_boom,Boom);

	virtual void doFly();
	void doBoom(Node* ref);

	void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);	

};

//炮兵700002
class SoldierPb : public Soldier
{
public:
	SoldierPb(int id);
	~SoldierPb(void);

	void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);	

	//void flyMove(Coord end);//弹道飞行
	//void runBoomAction();//爆炸效果

	virtual void doFly();
	void doBoom(Node* ref);

//private:
//	Armature *_fly;
//	Armature *_boom;
};

//护士700003
class SoldierHs : public Soldier
{
public:
	SoldierHs(int id);
	~SoldierHs(void);
	void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);
	//virtual void setTarget(int target);//设置目标
	virtual void findCandidateTarget(vector<BattleLogic*> &candidate);
	virtual BattleLogic* findTargetFromCandidate(vector<BattleLogic*> candidate);//查找目标
	void doBoom();
};

//自杀机器人700004
class SoldierZsjqr : public Soldier
{
public:
	SoldierZsjqr(int id);
	~SoldierZsjqr(void);

	void findCandidateTarget(vector<int> &candidate);

	virtual void animationEvent( Armature *armature, MovementEventType movementType, const string& movementID );



};

//火枪兵700005
class SoldierHqb : public Soldier
{
public:
	SoldierHqb(int id);
	~SoldierHqb(void);

	//void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);

private:

};
#endif // __HUMANSOLDIER_H__
