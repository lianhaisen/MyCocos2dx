#ifndef  _HEROCONFIG_H
#define  _HEROCONFIG_H

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
//Ӣ����Ϣ
class HeroInfo
{
public:
	//Ӣ������
	int id;
	//����
	int race;
	//����
	std::string name;
	//��������
	int attacktype;
	//��λ����
	int unittype;
	//����ƫ��
	int attackpre;
	//�˺�����
	int hurttype;
	//AOE��Χ
	int AOErange;
	//��������
	int burstscount;
	//��С����
	int attackmin;
	//��󹥻�
	int attackmax;
	//�����ٶ�
	int attackspeed;
	//�ƶ��ٶ�
	int movespeed;
	//����뾶
	int alertradius;
	//��ռ�˿�
	int population;
	//������
	int energypoint;
	//�����ȼ�
	int mainlv;
	//����ʯ
	int energystone;
	//����ID
	int skillid;
	//��������
	std::string skillname;
	//��������ʱ��
	int bulletflytime;
public:
	static map<int,HeroInfo*> getHeroConfig();
	static HeroInfo* getHeroFromId(int id);
private:
	static void initHeroConfig();

};

//Ӣ�ۻ���������Ϣ
class HeroBaseUpgrade
{
public:
	//�ȼ�
	int id;
	//��������
	int costtype;
	//���ĵĶ���
	int cost;
	//����ʱ��
	int costtiem_m;
	//Ӣ������ʱ��
	int rebirthtime_m;
	//����ֵ
	int honor;
public: 
	static map<int,HeroBaseUpgrade*> getHeroBaseUpgrade();
	static HeroBaseUpgrade* getHeroBaseFromId(int id);
private:
	static void initHeroBaseUpgrade();
};

//Ӣ������
class HeroUpgrade
{
public:
	HeroUpgrade();
	~HeroUpgrade();
public:
	//Ӣ��ID
	int id;
	//����
	int type;
	//����
	std::string name;
	//�˺�
	int hurt;
	//Ѫ��
	int hp;
    //��û�����Ϣ
	HeroBaseUpgrade * getBaseInfo();
private:
	HeroBaseUpgrade * _baseinfo;

public:
	static map<int,HeroUpgrade*> getHeroUpragrage();
	static HeroUpgrade* getHeroUpgrageFromId(int id);
private:
	static void initHeroUpragrade();
};

//Ӣ�ۼ�������
class HeroSkillUpgrade
{
public:
	//����ID
	int id;
	//��������
	int type;
	//��������
	std::string name;
	//��������
	int costtype;
    //����
	int cost;
	//����ʱ��
	int costtime;
    //Ч��ʱ��
	int effecttime;
    //��������
	int attackadd;
	//�ƶ�����
	int moveadd;
	//��ը�˺�
	int blowhurt;
	//��������˺�
	int turnedloseinjury;
	//ÿ������
	int treat;
	//���ƴ���
	int treatcount;
	//���Ƽ��
	int treatinterval;
	//���͹���
	int reduceattackspeed;

public:
	static map<int,HeroSkillUpgrade*> getHeroSkillUpgrade();
	static HeroSkillUpgrade* getHeroSkillFromId(int id);
private:
	static void initHeroSkillUpgrade();
};

//һ��Ӣ��ֻ����ļһ��
class HeroLevelAndSkillLevel
{
public:
	HeroLevelAndSkillLevel();
public:
	//Ӣ������ID
	int id; 
	//Ӣ�۵ȼ�
	int herolevel;
	//���ܵȼ�
	int skilllevel;
	//״̬
	int status;
    //0 Ӣ�� 1����
	int isupgradehero;
	//ʱ���
	long long timestamp;

private:
	//�ȼ�ת��
	int level;
	void getRealLevel();
	void getToLevel();
public:
	static map<int,HeroLevelAndSkillLevel*> getHeroLevelAndSkillLevel();
	static HeroLevelAndSkillLevel* getHeroLevelAndSkillLevelFromId(int id);
	static void createHeroItem(int nType,int nLevel = 1);
	int getTotalTime();
	bool setUpData(int nUpgradeHero);
private:
	static void init();
};

//   Ӣ�ۼ�̳������Ϣ
class HeroAltarConfig
{
public:
	HeroAltarConfig();
	void UpdataHeroToAltar(int nType);
public:
	//Ӣ�ۼ�̳ID
	int id;
	//Ӣ������ID
	int type;
public:
	static map<int,HeroAltarConfig*> getHeroAltarConfig();
	static HeroAltarConfig* getHeroAltarConfigFromId(int id);
	static void createHeroToAltar(int nId,int nType);

private:
	static void init();

};


#endif



