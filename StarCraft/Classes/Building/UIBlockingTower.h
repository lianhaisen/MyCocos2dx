#ifndef  _UIBLOCKINGTOWER_H
#define  _UIBLOCKINGTOWER_H

#include "UILightingTower.h"

class UIBlockingTower:public UILightingTower
{
public:
	UIBlockingTower();
	~UIBlockingTower();
	CREATE_FUNC(UIBlockingTower);
private:
	CC_SYNTHESIZE_RETAIN(Armature*,_attackArmature,AttackArmature);
	virtual void doFly();
	void getHeadIdByTargetCoord(Coord co);
	virtual void doAttack();
	std::string getHeadStringFromId(int id);
	int _headId;

};


#endif



