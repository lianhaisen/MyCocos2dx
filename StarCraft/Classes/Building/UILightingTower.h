#ifndef __UILIGHTINGTOWER_H
#define __UILIGHTINGTOWER_H
#include "UIBuildingBase.h"

// 这里暂时用最全的资源电塔
class UILightingTower : public Building
{
public:
	UILightingTower();
	~UILightingTower();
	CREATE_FUNC(UILightingTower);
	virtual bool initWithBattleLogic( DBuilding * db );
	virtual void updateStatus();
	
	//获得放置层
	Node * getPlaceLayer();
	//设置目标点
	void setTmpPt(Point pt){_tempPoint = pt;}
	//获得目标点
	virtual Point getTmpPt();

	virtual bool isTouched(float dis);
	virtual bool isInAoeHurt(float dis);
	virtual float getFlyTime(float distance);

private:
	CC_SYNTHESIZE_RETAIN(Armature*,_flyArmature,FlyArmature);
	CC_SYNTHESIZE_RETAIN(Armature*,_boomArmature,BoomArmature);
	virtual void  doAttack();
	virtual void  doFly();
	virtual void  doBoom();

	virtual void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
	virtual BattleLogic* findTarget();//查找目标
	//初始化扫描队列
	void initWatchlist();
	//目标是否在攻击范围内
    bool isTargetInFightArea(NewCoord * target);
	//攻击定时器
	void updateAttack(float dt);
	//侦查定时器
	virtual void updateLogic(float dt);
private:
	std::list<NewCoord*> _watchilist;
	Point _tempPoint;
};


#endif



