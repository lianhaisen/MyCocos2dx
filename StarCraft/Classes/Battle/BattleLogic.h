#ifndef __BACEOBJECT_H__
#define __BACEOBJECT_H__
#include "MapHelper.h"

#define E_STATUS_DEAD 99

//ս��״̬
enum E_BattleStatus
{
	e_battle_status_idle = 0,				//����
	e_battle_status_relax = 1,				//����
	e_battle_status_path = 2,				//��ʼѰ·
	e_battle_status_pathing = 3,			//Ѱ·�У����ҹ���Ŀ�꣬��Ѱ·����δ��ʼ�ƶ���
	e_battle_status_move = 4,				//��ʼ�ƶ�
	e_battle_status_moveing = 5,			//�ƶ���
	e_battle_status_attack = 6,				//��ʼ����
	e_battle_status_attacking = 7,			//������
	e_battle_status_dead = E_STATUS_DEAD,	//����
	e_battle_status_deaded = 100			//������
};

//����
enum E_BattleDir
{
	e_battle_dir_none = 0,		//��
	e_battle_dir_east = 1,		//��
	e_battle_dir_es = 2,		//����
	e_battle_dir_south = 3,		//��
	e_battle_dir_ws = 4,		//����
	e_battle_dir_west = 5,		//��
	e_battle_dir_wn = 6,		//����
	e_battle_dir_north = 7,		//��
	e_battle_dir_en = 8,		//����
};

//����Ŀ�����ͣ��Կգ��Եء�����
enum E_Attack_Target
{
	e_attack_target_none,//���ܹ���
	e_attack_target_air,//�Կ�
	e_attack_target_ground,//�Ե�
	e_attack_target_all,//�ԿնԵ�
	e_attack_target_ground2//�Եظ���
};

enum E_Attack_Preference
{
	e_preference_none,//��
	e_preference_life,//������λ
	e_preference_machine,//��е��λ
	e_preference_tower,//����
	e_preference_resource,//��Դ����
	e_preference_normal,//��ͨ����
	e_preference_facility,//��ʩ
	e_preference_all1,//ȫ������(��ͨ����+��Դ����)
	e_preference_all2,//ȫ����������ʩ
	e_preference_all3,//ȫ������������
	e_preference_trap,//����
	e_preference_barrier1,//���Ƴ��ϰ�
	e_preference_barrier2//�����Ƴ��ϰ�
};

enum E_Unit_Type
{
	e_unit_type_none,//��
	e_unit_type_ground,//����
	e_unit_type_air,//����

};

class BattleLogic;
struct NewCoord
{
	int key;//x*MAX_GRID_COUNT+y
	//BattleLogic* soldier;
	vector<BattleLogic*> soldierlist;
};

class BattleLogic
{
public:
	BattleLogic(void);
	~BattleLogic(void);
/**************************************************
ͨ�÷���
**************************************************/
	virtual int getId();
	virtual void setId(int id);
	virtual int getType();
	virtual void setType(int type);
	virtual E_BattleStatus getStatus();
	virtual void setStatus(E_BattleStatus status);
	virtual Coord getCoord();//�������ĵ�����
	virtual void setCoord(Coord coord);
	virtual int getHp();//��ȡ��ǰhp
	virtual void setHp(int hp);//��ʼ��hp��maxHP
	//��Ѫ�������Ƿ���������ʿ��Ѫ������Ѫ�Ƿ����
	virtual bool subHp(int hp);
	virtual bool isFullHp();//�Ƿ���Ѫ
	virtual void setTarget(BattleLogic* target);//����Ŀ��
	virtual BattleLogic* getTarget();//��ȡĿ��
    
	//��������� ����Ҫ�����������Ĳ�Ҫ����
	virtual NewCoord* getNewCoord(){return nullptr;}
	//ִ�ж��� 
	virtual void  startLogic(){} 
	

	virtual void updateHpProgress(){};//����Ѫ���ٷֱ�
	virtual void dead(){};

	//���úͻ���ĵ�
	virtual void  setMidCoord(Coord coord){_midcoord = coord;}
	virtual Coord getMidCoord(){return  _coord;}

protected:
	unsigned int _id;//ÿ��ʿ����Ψһ��ʶ
	unsigned int _typeid;
	E_BattleStatus _status;
	Coord _coord;
	Coord _midcoord;
	int _maxHp;//���hp
	int _hp;//��ǰhp
	BattleLogic *_target;//Ŀ�� TODO:����Ŀ�껹�п����ǵз�ʿ����Ӣ��

};






#endif // __BACEOBJECT_H__