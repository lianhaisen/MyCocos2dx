#ifndef __SOLDIERCONFIG_H__
#define __SOLDIERCONFIG_H__
#include <string>
#include <vector>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;


class CSoldier
{
public:
	int id;						//��ʶ������+�ȼ���
	string name;				//����
	int type;					//���ͱ�ʶ
	//int level;				//�ȼ�
	int attack_type;			//�������ͣ����壬AOE��
	int preference;				//����ƫ��
	int attack_target;			//����Ŀ�����ͣ��Կգ��Եأ�
	float aoe_range;			//AOE��Χ
	int min_range;				//��С������Χ
	int max_range;				//��󹥻���Χ
	float attack_speed;			//�����ٶ�S
	int attack;					//����ֵ
	int max_hp;						//����ֵ
	int dot;					//�����˺�
	int dot_time;				//�����˺�ʱ��S
	float move_speed;			//�ƶ��ٶ�
	int population;				//ռ�˿�
	int air_population;			//�����˿�
	int train_time;				//ѵ��ʱ��S
	int is_air;					//����
	int is_invisible;			//����
	int is_jump;				//��Ծ
	int camp_level;				//��Ӫ�ȼ�
	int lab_level;				//��ѧʵ���ҵȼ�
	int building_id;			//��������ID	
	int skill_type;				//��������ID
	int cost_type;				//������Դ����
	int train_cost;				//����������Դ����
	int upgrade_cost;			//����������Դ����
	int upgrade_time;			//����ʱ��S
	int race;					//����
	float power_points;			//������
	int chip;					//��Ƭ����	
	int unit_type;				//��λ���ͣ����棬���У�
	int life_type;				//�������ͣ���������е��
	int soldier_points;			//���ֻ���
	float fly_speed;			//���������ٶ�
};


class SoldierConfig
{
public:
	SoldierConfig(void);
	~SoldierConfig(void);
public:
	static map<int, CSoldier*> getSoldierConfig();
	static CSoldier* getSoldierConfig(int id);
private:
	static void initSoldierConfig();

};

//�û���ǰ�����������
class UserSoldierLevel
{
public:
	UserSoldierLevel();
	~UserSoldierLevel(){}
public:
	//����
	int id;
	//�ȼ�
	int level;
    
	//���µȼ�
	void updateLevel();

	CSoldier * getCurcfg();
	CSoldier * getNextcfg();
private:
	CSoldier * _curcfg;
	CSoldier * _nextcfg;
public:
	static map<int,UserSoldierLevel*> getUserSoldierLevels();
	static UserSoldierLevel * getUserSoldierLevelFromId(int id);
private:
	static void initSoldierLevels();

};



#endif //__SOLDIERCONFIG_H__