#ifndef  _UIPIERCINGCANNON_H
#define  _UIPIERCINGCANNON_H

#include "UILaserCannon.h"
//穿甲炮
class UIPiercingCannon : public UILaserCannon
{
public:
	UIPiercingCannon();
	~UIPiercingCannon();;
	CREATE_FUNC(UIPiercingCannon);
	//获得空闲弹道
    Armature * getIdleFly();
	//获得空闲爆炸
	Armature * getIdleBoom();

	virtual void animationEvent( cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID );
	virtual void frameEvent( Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex );
	virtual void doFly();
	virtual void doBoom(Point pt);

	
    virtual void flyEndCallBack(Node* ref);

	virtual bool initWithBattleLogic( DBuilding * db );
	//更新弹道路径
	virtual void  updateFlyWays(float dt);

	Vector<Armature*> _flys;
	Vector<Armature*> _booms;

};



#endif



