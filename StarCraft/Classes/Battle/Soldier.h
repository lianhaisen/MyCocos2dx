#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "BattleLogic.h"
#include "MapHelper.h"

using namespace cocos2d;
using namespace cocostudio;


//ʿ������
class Soldier : public Layer, public BattleLogic
{
public:
	Soldier(){}
	Soldier(int id);
	virtual ~Soldier(void);
	//CC_SYNTHESIZE_RETAIN(Armature*,_body,Body);
	Armature* getBody();
	void setBody(Armature* body);
	Armature * _body;

	//CC_SYNTHESIZE_RETAIN(Armature*,_head,HeadArmature);
	CC_SYNTHESIZE_RETAIN(ProgressTimer*,_hpProcess,HPProgress);
	CC_SYNTHESIZE_RETAIN(Sprite*,_hpBg,HPBg);
	
	void createBody();
	void createHpProgress();//����Ѫ��
	void showUI();//��ʾʿ��
	Armature* getIdleFly();
	Armature* getIdleBoom();


	void startLogic();//������ʱ��
	void updateLogic(float dt);//ʿ���߼�
	void updateAttack(float dt);//������ʱ��
	virtual NewCoord* getNewCoord();

	virtual void setCoord(Coord coord);
	/*
	 *���Һ�ѡ����Ŀ�꣬���3��
	 *��3��Ŀ����ѡ��һ��������С��Ϊ����Ŀ��
	 *�����ϰ��ֱ�Ӽ�������֮��ľ���
	 */
	virtual void findCandidateTarget(vector<BattleLogic*> &candidate);
	virtual BattleLogic* findTargetFromCandidate(vector<BattleLogic*> candidate);//����Ŀ�꣨��ʿ��Ŀ�����ѷ�ʿ������Ѱ·
	virtual void doMove();//��Ŀ���ƶ�
	virtual void doAttack();//����Ŀ�꣬��ʿ��ΪĿ���Ѫ
	virtual void doFly(){};//����
	virtual void doBoom(){};//��ըЧ��
	virtual bool subHp( int hp );
	virtual void updateHpProgress();
	virtual void dead();
	virtual void runWalkAction();//��·����
	virtual void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);
	virtual void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

protected:
	int getDirection();
	void setPath(vector<Coord> path);
	Coord getNextCoord();//��ȡѰ·����һ�������
	bool setMoveDirection(Coord next);//�����ƶ����򣬷����Ƿ�ı䷽��
	void setAttackDirection();//���ù�������
	//TODO:��Ϊ���Ƕ�̬���㣬�����ж��Ƿ���Ԥ���趨�ĵ���
	bool checkDistance();//���Ŀ���Ƿ��ڹ�����Χ��
	void followTarget();//����Ŀ��
	string getArmatureName();

	void release();
public:
	unsigned int m_pathIndex;//��ǰ·���������
	unsigned int m_direction;//����
	string m_attackName;
	vector<Coord> m_path;
	NewCoord* _newCoord;
	Vector<Armature*> _flys;
	Vector<Armature*> _booms;

};

#endif // __SOLDIER_H__
