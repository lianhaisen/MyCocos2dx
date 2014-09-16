#include "UIMulrocket.h"
#include "BuildingCommon.h"
#include "BuildingConfig.h"
#include "AppMacros.h"

void UIMulrocket::doFly()
{   

	
	auto fly1 = getIdleFly();
	getPlaceLayer()->addChild(fly1,(MAX_GRID_COUNT + 1) * (MAX_GRID_COUNT + 1));
	auto fly2 = getIdleFly();
	getPlaceLayer()->addChild(fly2,(MAX_GRID_COUNT + 1) * (MAX_GRID_COUNT + 1));

	auto bone1 = getFortBuilding()->getBone("point1");
	auto bone2 = getFortBuilding()->getBone("point2");

	auto towerpoint   = MapHelper::getPointFromCoord(getCoord().x,getCoord().y);
	auto soldierpoint = getTmpPt();

	auto targetp1     = towerpoint;
	auto targetp2     = towerpoint;

	targetp1.x +=  bone1->getWorldInfo()->x * getBuildingArmature()->getScale();
	targetp1.y +=  bone1->getWorldInfo()->y * getBuildingArmature()->getScale();

	targetp2.x +=  bone2->getWorldInfo()->x * getBuildingArmature()->getScale();
	targetp2.y +=  bone2->getWorldInfo()->y * getBuildingArmature()->getScale();

	fly1->setPosition(targetp1);
	fly2->setPosition(targetp2);

	auto pointNomal1 = soldierpoint - targetp1;
	auto vr1 = pointNomal1.getNormalized();

	auto pointNomal2 = soldierpoint - targetp2;
	auto vr2 = pointNomal2.getNormalized();

	fly1->setRotation(atan2(vr1.x,vr1.y) * 180 / M_PI  - 90);
	fly2->setRotation(atan2(vr2.x,vr2.y) * 180 / M_PI  - 90);

	fly1->getAnimation()->play(BuildingCommon::getFlystring(getLevel()),-1,1);
	fly2->getAnimation()->play(BuildingCommon::getFlystring(getLevel()),-1,1);

	auto distance1 = pointNomal1.getLength();
	auto distance2 = pointNomal2.getLength();

	auto action1 = Sequence::createWithTwoActions(MoveTo::create( getFlyTime(distance1),soldierpoint),
		CallFuncN::create(CC_CALLBACK_1(UIPiercingCannon::flyEndCallBack,this))
		);
	fly1->runAction(action1);

	auto action2 = Sequence::createWithTwoActions(MoveTo::create( getFlyTime(distance2),soldierpoint),
		CallFuncN::create(CC_CALLBACK_1(UIPiercingCannon::flyEndCallBack,this))
		);
	fly2->runAction(action2);

	
}


