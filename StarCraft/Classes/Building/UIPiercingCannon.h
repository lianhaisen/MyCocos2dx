#ifndef  _UIPIERCINGCANNON_H
#define  _UIPIERCINGCANNON_H

#include "UILaserCannon.h"
//������
class UIPiercingCannon : public UILaserCannon
{
public:
	UIPiercingCannon();
	~UIPiercingCannon();;
	CREATE_FUNC(UIPiercingCannon);
	//��ÿ��е���
    Armature * getIdleFly();
	//��ÿ��б�ը
	Armature * getIdleBoom();

	virtual void animationEvent( cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID );
	virtual void frameEvent( Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex );
	virtual void doFly();
	virtual void doBoom(Point pt);

	
    virtual void flyEndCallBack(Node* ref);

	virtual bool initWithBattleLogic( DBuilding * db );
	//���µ���·��
	virtual void  updateFlyWays(float dt);

	Vector<Armature*> _flys;
	Vector<Armature*> _booms;

};



#endif



