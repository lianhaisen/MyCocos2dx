#ifndef __AIRSOLDIER_H__
#define __AIRSOLDIER_H__
/*
*ʵ�ַɻ���������ʿ������������ֱ����������ս�����������������ս������ӥս����ս�н�
*/
#include "Soldier.h"


//����ֱ����700011
class SoldierDnzsj : public Soldier
{
public:
	SoldierDnzsj(int id);
	~SoldierDnzsj(void);

	virtual void runWalkAction();//��·����
};

//����ս��700012
class SoldierYlzj : public Soldier
{
public:
	SoldierYlzj(int id);
	~SoldierYlzj(void);

	virtual void runWalkAction();//��·����
};

//�����700013
class SoldierYsj : public Soldier
{
public:
	SoldierYsj(int id);
	~SoldierYsj(void);

	virtual void runWalkAction();//��·����
};

//������ս��700014
class SoldierHbfzj : public Soldier
{
public:
	SoldierHbfzj(int id);
	~SoldierHbfzj(void);

	virtual void runWalkAction();//��·����
};

//��ӥս��700015
class SoldierTyzj : public Soldier
{
public:
	SoldierTyzj(int id);
	~SoldierTyzj(void);

	virtual void runWalkAction();//��·����
};

//ս�н�700016
class SoldierZlj : public Soldier
{
public:
	SoldierZlj(int id);
	~SoldierZlj(void);

	virtual void runWalkAction();//��·����
};
#endif // __AIRSOLDIER_H__
