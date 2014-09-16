#ifndef  _BARRACKDATA_H
#define  _BARRACKDATA_H

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

#include "SoldierConfig.h"

//TODO ������б������ּӶ��Ÿ���

//���ֶ���
class BarrackItem
{
public:
	int id;
	int soldierid;
	int number;
	int population;
	//ʿ����ʹ������
	int soldierstate;
};
class BarrackData
{
public:
	//������ID
	int id;
	//����˿�
	int maxpopulation;
	//��ǰ�˿�
	int curpopulation;
	//ʿ���б�
	list<BarrackItem*> soldieritems;

	void addSoldier(int id);

	void updateData();

	void updataSoldierUseState(int Soldierid,int Barrackid,int nNum);


private:
	//ʿ�������б�
	string soldiertypeids;
	//ʿ����Ŀ�б�,һһ��Ӧ
	string soldiernums;
	//ÿ������ռ�˿�
	string soldiersharepopulations;
	//ʿ��ʹ��״̬
	string soldierstates;
public:
	static map<int,BarrackData*> getBarrackDataMap();
	static BarrackData * getBarrackFromId(int id);
	static BarrackData * createNew(int id);

private:
	static void initBarrackData();

};

class TrainItem
{
public:
	int soldierid;
	int number;
	int trainingtime;
	int population;
};

class TrainingData
{
public:
	//������ID
	int id;
	//����˿�
	int maxpopulation;
	//��ǰ�˿�
	int curpopulation;
	//ʿ���б�
	list<TrainItem *> soldieritems;
	//��ǰ���ʣ��ʱ��
	int curtraininglavetime;
	//�����һ����Ҫ��ʱ��
	int getNextCanBuildTime();

	void deletaitems(); 

	//����ܹ�ʱ��
	int getTotalTime();
	//��������
	void updateData();
private:
	//ʿ�������б�
	string soldiertypeids;
	//ʿ�������б�
	string soldiernums;
	//ʿ��ѵ��ʱ��
	string soldiertrainingtimes;
	//ʿ��ռ�˿��б�
	string soldiersharepopulations;
public:
    static map<int,TrainingData*> getTrainingDataMap();
	static TrainingData * getTrainingDataFromId(int id);
	static TrainingData *CreateItem(int id);
private:
	static void initTrainingData();

};

class SoldierUpgradeData
{
public:
	SoldierUpgradeData();
	~SoldierUpgradeData();
public:
	//������ID
	int id;
	//��������
	int type;
	//����״̬ 0 ���� 1������
	int status;
	//�������ʱ���
	long long timestamp;
	UserSoldierLevel * getSoldierLevel();
    int getLeftTime();
private:
	UserSoldierLevel * _userLevel;

public:
	static map<int,SoldierUpgradeData*> getSoldierUpgradeMap();
	static SoldierUpgradeData* getSoldierUpgradeMapFromId(int id);
	static void UpgradeSoldier(int bid,int soldiertype,int time);
private:
	static void initSoldierUpgradeData();

};

//�������Ϣ
class ConveyorConfig
{
public:
	//�����ID
	int id;
	//������
	int soldierAllNum;
	//ʿ����Ϣ
	list<BarrackItem*> soldieritems;

	//����
	void addSoldier(int barrackId,int soldierid); 
	//����
	void DeleteSoldier(int barrackId,int soldierid );
private:
	//��ӪID
	string barrackIds;
	//ʿ�������б�
	string soldiertypeids;
	//ʿ�������б�
	string soldiernums;
public:
	static map<int,ConveyorConfig*> getConveyorConfig();
	static ConveyorConfig* getConveyorConfigFromId(int id);
	static void CreateItem(int id);

private:
	static void initConveyorConfig();
	//��������
	void updateData();
};

#endif



