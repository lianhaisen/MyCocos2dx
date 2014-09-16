#ifndef  __UITOWER_H
#define  __UITOWER_H

//ÅÚËþÀà
#include "UILightingTower.h"

class UILaserCannon :public  UILightingTower
{
public:
	UILaserCannon();
	~UILaserCannon();
	CREATE_FUNC(UILaserCannon); 
	virtual void updateStatus();
private:
	CC_SYNTHESIZE_RETAIN(Armature*,_Fort,FortBuilding);
	void CreateFordBuilding();

	int getMovement(int from, int to);

	virtual	void getHeadIdByTargetCoord(Coord co);
	virtual void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
	virtual void frameEvent(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex);
	virtual void doAttack();
	virtual void doFly();

	int _degreeIndex;
	int _currentIndex;
};


#endif



