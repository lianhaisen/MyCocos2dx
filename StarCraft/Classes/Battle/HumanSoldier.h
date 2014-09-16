#ifndef __HUMANSOLDIER_H__
#define __HUMANSOLDIER_H__
/*
*ʵ�ֱ�����������ʿ����������ǹ�����ڱ�����ʿ����ɱ�����ˡ���ǹ��
*/
#include "Soldier.h"


//��ǹ��700001
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

//�ڱ�700002
class SoldierPb : public Soldier
{
public:
	SoldierPb(int id);
	~SoldierPb(void);

	void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);	

	//void flyMove(Coord end);//��������
	//void runBoomAction();//��ըЧ��

	virtual void doFly();
	void doBoom(Node* ref);

//private:
//	Armature *_fly;
//	Armature *_boom;
};

//��ʿ700003
class SoldierHs : public Soldier
{
public:
	SoldierHs(int id);
	~SoldierHs(void);
	void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);
	//virtual void setTarget(int target);//����Ŀ��
	virtual void findCandidateTarget(vector<BattleLogic*> &candidate);
	virtual BattleLogic* findTargetFromCandidate(vector<BattleLogic*> candidate);//����Ŀ��
	void doBoom();
};

//��ɱ������700004
class SoldierZsjqr : public Soldier
{
public:
	SoldierZsjqr(int id);
	~SoldierZsjqr(void);

	void findCandidateTarget(vector<int> &candidate);

	virtual void animationEvent( Armature *armature, MovementEventType movementType, const string& movementID );



};

//��ǹ��700005
class SoldierHqb : public Soldier
{
public:
	SoldierHqb(int id);
	~SoldierHqb(void);

	//void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);

private:

};
#endif // __HUMANSOLDIER_H__
