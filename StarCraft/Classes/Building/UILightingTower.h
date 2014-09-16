#ifndef __UILIGHTINGTOWER_H
#define __UILIGHTINGTOWER_H
#include "UIBuildingBase.h"

// ������ʱ����ȫ����Դ����
class UILightingTower : public Building
{
public:
	UILightingTower();
	~UILightingTower();
	CREATE_FUNC(UILightingTower);
	virtual bool initWithBattleLogic( DBuilding * db );
	virtual void updateStatus();
	
	//��÷��ò�
	Node * getPlaceLayer();
	//����Ŀ���
	void setTmpPt(Point pt){_tempPoint = pt;}
	//���Ŀ���
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
	virtual BattleLogic* findTarget();//����Ŀ��
	//��ʼ��ɨ�����
	void initWatchlist();
	//Ŀ���Ƿ��ڹ�����Χ��
    bool isTargetInFightArea(NewCoord * target);
	//������ʱ��
	void updateAttack(float dt);
	//��鶨ʱ��
	virtual void updateLogic(float dt);
private:
	std::list<NewCoord*> _watchilist;
	Point _tempPoint;
};


#endif



