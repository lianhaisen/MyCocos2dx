#ifndef __BUILDINGDATA_H__
#define __BUILDINGDATA_H__
#include <string>
#include <vector>
#include "cocos2d.h"
#include "MapHelper.h"


using namespace std;
using namespace cocos2d;

class CBuilding;
class Building;


class DBuilding
{
public:
	enum class BuildingStatus
	{   
		//����
		RELEX  = 0,
		//������
		UPGRADE = 1,
		//����
		WORK = 2,
		//�½�
		NewBuild = 3
	};
public:
	DBuilding();
	~DBuilding();

public:
	int id;
    int type; //����
	Coord coord;//��������
	BuildingStatus status;
	long long timestamp;//ʱ���
	int  curpro; //��ǰ����
public:
	
	//************************************
	// Method:    updateData ��������
	// Parameter: int type ����ID
	void updateCoor(int x,int y);
	//��������
	bool setUpgrade();
	//���ʣ��ʱ��(���������½�)
	int getLeftTime(); 

	//���²���
	void updatePro();

    
	//��õ�ǰ�ȼ��İ�����
    CBuilding * getCurBindCfg();
	//�����һ�ȼ��İ�����
	CBuilding * getNextBindCfg();
	//������ȼ��İ�����
	CBuilding * getMaxBindCfg();

	//��ý�����
	Building * getBuilding(){return _building;}
	//���ý�����
	void setBuilding(Building * building){_building = building;}

private:
	CBuilding * _curCfg;
	CBuilding * _nextCfg;
	CBuilding * _maxCfg;
	Building * _building;
};

class BuildingData
{
public:
	BuildingData(void);
	~BuildingData(void);
public:
	static vector<DBuilding*> getBuildingData();
	static DBuilding* getBuildingData(int id);

	//************************************
	// Method:    �����½�������
	// FullName:  BuildingData::CreateDBuilding
	// Access:    public static 
	// Returns:   DBuilding*
	// Qualifier:
	// Parameter: int type ��������id
	// Parameter: int x    
	// Parameter: int y
	//************************************
	static DBuilding* CreateDBuilding(int type,int x,int y);
	


private:
	static void initBuildingData();
};


#endif //__BUILDINGDATA_H__