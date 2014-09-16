#ifndef __BUILDINGCONFIG_H__
#define __BUILDINGCONFIG_H__
#include <string>
#include <vector>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

/*
//����Ŀ��
enum E_AttackTarget
{
	e_none,
	e_ground,
	e_air,
	e_all
};
//��������
enum E_AttackType
{
	e_none,
	e_single,
	e_aoe
};
//����ƫ��
enum E_AttackPreference
{
	e_none,
};
*/


class CBuilding
{
public:
	enum class ResourceType
	{   
		//��ˮ��
		BLUE = 1,
		//��ˮ��
		BLACK = 2,
		//����
		GOLD = 3,
		//����ʯ
		STONE = 4,
		//��ʯ
		DIAMOND = 5
	};

public:
	int id;						//��ʶ������+�ȼ���
	string name;				//����
	int type;					//�������ͱ�ʶ
	int grid;					//��������
	int level;					//�ȼ�
	int townhall;				//�󱾵ȼ�
	int cost_time;				//����ʱ��S
	ResourceType cost_type;		//����������Դ����
	int cost_value;				//������Դ
	int max_hp;					//����ֵ
	int attack;					//����ֵ
	//��������
	int attack_target;			//����Ŀ�����ͣ��Կգ��Եأ�
	int preference;				//����ƫ��
	int attack_type;			//��������
	int aoe_range;				//AOE��Χ
	int min_range;				//��С������Χ
	int max_range;				//��󹥻���Χ
	int attack_speed;			//�����ٶ�S
	int defense_points;			//��������
	//��ʩ����
	int sub_attack;				//�����˺�%
	int add_attack;				//�����˺�ֵ%
	//��������
	int cost_repair;			//������Դ
	int trap_type;				//��������
	int trap_effect;			//����Ч��
	int attack_range;			//�˺��뾶
	int trigger_range;			//�����뾶
	int trap_delay;				//��ը��ʱS
	int slow_time;				//����ʱ��S
	int slow_speed;				//�ٶȽ���%
	int stun_time;				//ѣ��ʱ��S
	int freeze_time;			//����ʱ��S
	int smoke_time;				//����ʱ��S
	int poison_time;			//�ж�ʱ��S
	int poison_attack;			//�ж��˺�
	//��������
	int alert_range;			//���䷶Χ
	int storage_type;			//�洢����
	int storage;				//�洢����
	int output;					//Сʱ����H
	int population;				//�����˿�
	//�����ֶ�
	int race;					//����
	int power_points;			//������
	int command;				//С��ͳ��
	int exp;					//����ֵ
	int building_type;			//�������ͣ���������Դ��������ͨ��������ʩ�����ڹ���ƫ��
	float fly_speed;			//���������ٶ�
};


class BuildingConfig
{
public:
	BuildingConfig(void);
	~BuildingConfig(void);
public:
	static map<int, CBuilding*> getBuildingConfig();
	static CBuilding* getBuildingConfig(int id);
private:
	static void initBuildingConfig();

};


class OpenBuilding
{
public:
	enum class BuyBuildingType
	{   
		//��������
		FONDATION = 1,
		//��������
		DEFENSE,
		//��Դ����
		RESOURCE,
		//���彨��
		TRAP,
	};

public:
	//����
	int id;
	//��������
	BuyBuildingType type;
	//�����ȼ�
	int openlevel;
	//��1
	int level1;
	int level2;
	int level3;
	int level4;
	int level5;
	int level6;
	int level7;
	int level8;
	int level9;
	int level10;
	int getLimitCountFormLevel(int level);
public:
	static std::vector<OpenBuilding*> getData();
	static void initData();
};

class BuyListConfig
{
public:
	CBuilding * buildcfg;
	int current;
	OpenBuilding * opencfg;
public: 
	BuyListConfig();
	~BuyListConfig(){}
    BuyListConfig(CBuilding* scfg);
    static std::vector<BuyListConfig*> getBuyListFromType(OpenBuilding::BuyBuildingType type = OpenBuilding::BuyBuildingType::FONDATION);
	static BuyListConfig * getBuyListFromId(int id);

private:
    static void initBuyList();
};







#endif //__BUILDINGCONFIG_H__