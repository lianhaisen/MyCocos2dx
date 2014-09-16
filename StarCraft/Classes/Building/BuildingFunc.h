#ifndef __BUILDINGFUNC_H
#define __BUILDINGFUNC_H
using namespace std;
#include "BuildingData.h"
#include "UIUserMinUi.h"
#include "SoldierConfig.h"
#include "BarrackData.h"

//����Դ���߼�

// ��Ҫȥ����ʿ��
class SolidToBuild
{
public:
	//ʿ��ID
	int solidId;
	//��Ŀ
	int num;
	//��ǰ����ʱ��
	int curpass;
};

class BuildingFunc
{
private:
	BuildingFunc();
	~BuildingFunc();
public:
	static BuildingFunc * getInstance();
	//ͳ�ƽ���
	void addContains(DBuilding * db);
	//�ռ���Դ
	void CollectResource(DBuilding * db);
	//�����������ȼ�
	int getMainLevel();
	//��ÿ�ѧԺ�ȼ�
	int getScienceLevel();
	void setMinUI(UIUserMinUi* ui);
	//���
	vector<CSoldier*> getSolidListFromDBuilding(DBuilding * db);
	//�������ʿ��
	map<int,BarrackItem*> getAllSoldier();

	//����ʿ��
	void addSoldier(Coord pos,int id,int popution);

	vector<DBuilding*> getBarracks(){return _barracks;}
	DBuilding* getBarrackFromIndex(int i);
	int        getBarrackIndexFormDB(DBuilding * db);


private:
	int getLeftCanStore(vector<DBuilding*> ve);
	vector<DBuilding*> getContainsFromType(int type);
	//�õ���������ɵ�
	int getMaxCanStore(vector<DBuilding*> ve);
	//�õ���ǰ������
	int getCurStroe(vector<DBuilding*>ve);

	void AddResourceToMainUI(int type,int count);
 
private:
	//��ˮ���ֿ�
	vector<DBuilding*> _blueContains;
	//�����ֿ�
	vector<DBuilding*> _goldContains;
	//��ˮ���ֿ�
	vector<DBuilding*> _blackContains;
	//������
	DBuilding*         _mainBuilding;
    
	map<int,BarrackItem*> _allSoldiers;
	//��Ӫ
	vector<DBuilding*> _camps;

	//��Ӫ(�����ĵط�)
	vector<DBuilding*> _barracks;
	
	//�о�Ժ
	DBuilding * _science;

	//��Ӫ�ɽ������
	map<DBuilding*,vector<CSoldier*>> _barracksBuildlist;

};



#endif



