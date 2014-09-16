#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "BattleLogic.h"
#include "MapHelper.h"

using namespace cocos2d;
using namespace cocostudio;


//士兵基类
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
	void createHpProgress();//创建血条
	void showUI();//显示士兵
	Armature* getIdleFly();
	Armature* getIdleBoom();


	void startLogic();//启动定时器
	void updateLogic(float dt);//士兵逻辑
	void updateAttack(float dt);//攻击定时器
	virtual NewCoord* getNewCoord();

	virtual void setCoord(Coord coord);
	/*
	 *查找候选攻击目标，最多3个
	 *从3个目标中选出一个代价最小作为攻击目标
	 *忽略障碍物，直接计算两点之间的距离
	 */
	virtual void findCandidateTarget(vector<BattleLogic*> &candidate);
	virtual BattleLogic* findTargetFromCandidate(vector<BattleLogic*> candidate);//查找目标（护士的目标是友方士兵）并寻路
	virtual void doMove();//向目标移动
	virtual void doAttack();//攻击目标，护士是为目标加血
	virtual void doFly(){};//弹道
	virtual void doBoom(){};//爆炸效果
	virtual bool subHp( int hp );
	virtual void updateHpProgress();
	virtual void dead();
	virtual void runWalkAction();//走路动作
	virtual void animationEvent(Armature *armature, MovementEventType movementType, const string& movementID);
	virtual void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

protected:
	int getDirection();
	void setPath(vector<Coord> path);
	Coord getNextCoord();//获取寻路的下一个坐标点
	bool setMoveDirection(Coord next);//设置移动方向，返回是否改变方向
	void setAttackDirection();//设置攻击方向
	//TODO:改为不是动态计算，而是判断是否在预先设定的点内
	bool checkDistance();//检查目标是否在攻击范围内
	void followTarget();//跟随目标
	string getArmatureName();

	void release();
public:
	unsigned int m_pathIndex;//当前路径点的索引
	unsigned int m_direction;//方向
	string m_attackName;
	vector<Coord> m_path;
	NewCoord* _newCoord;
	Vector<Armature*> _flys;
	Vector<Armature*> _booms;

};

#endif // __SOLDIER_H__
