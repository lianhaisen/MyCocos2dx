#ifndef __MACHINESOLDIER_H__
#define __MACHINESOLDIER_H__
/*
*ʵ�ֻ�е����������ʿ����������еս��������ս�������ǳ���̹�ˡ����������
*/
#include "Soldier.h"


//��еս��700006
class SoldierJxzj : public Soldier
{
public:
	SoldierJxzj(int id);
	~SoldierJxzj(void);

};

//����ս��700007
class SoldierLyzc : public Soldier
{
public:
	SoldierLyzc(int id);
	~SoldierLyzc(void);

	virtual void runWalkAction();//��·����
	virtual void doMove();//��Ŀ���ƶ�
	virtual void doAttack();//����Ŀ�꣬��ʿ��ΪĿ���Ѫ


private:
	CC_SYNTHESIZE_RETAIN(Armature*,_head,Head);
	double getBeginDegree();//��ʼ����ǰ�Ƕ�
	double getEndDegree();
	void rotate();

	int getIndexFormDegree(int degree);
	string getMovement(int from, int to);
	std::string getTowerStringFromLevel(int level, int degree = 1);
	void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);
private:
	//double _currentDegree;//��ǰ�Ƕ�
	//double _attackDegree;//�����Ƕ�
	
	int _degreeIndex;
	int _currentIndex;
};

//���ǳ�700008
class SoldierGcc : public Soldier
{
public:
	SoldierGcc(int id);
	~SoldierGcc(void);

	virtual void runWalkAction();//��·����
};

//̹��700009
class SoldierTk : public Soldier
{
public:
	SoldierTk(int id);
	~SoldierTk(void);

	virtual void runWalkAction();//��·����

private:
	CC_SYNTHESIZE_RETAIN(Armature*,_head,Head);
};

//���������700010
class SoldierLsjqr : public Soldier
{
public:
	SoldierLsjqr(int id);
	~SoldierLsjqr(void);

};
#endif // __MACHINESOLDIER_H__
