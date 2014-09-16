#include "UIBunkers.h"
#include "BuildingCommon.h"

void UIBunkers::doFly()
{
	auto fly = getIdleFly();
	auto bone = getFortBuilding()->getBone("point");
	auto towerpoint   = MapHelper::getPointFromCoord(getCoord().x,getCoord().y);
	towerpoint.x +=  bone->getWorldInfo()->x * getBuildingArmature()->getScale();
	towerpoint.y +=  bone->getWorldInfo()->y * getBuildingArmature()->getScale();
	auto soldierpoint = getTmpPt();
	auto pointNomal = soldierpoint - towerpoint;
	auto vr = pointNomal.getNormalized();

	fly->setPosition(towerpoint);
	fly->setRotation(atan2(vr.x,vr.y) * 180 / M_PI  - 90);
	fly->getAnimation()->play(BuildingCommon::getFlystring(getLevel()),-1,1);
	getPlaceLayer()->addChild(fly,(MAX_GRID_COUNT + 1) * (MAX_GRID_COUNT + 1));

	auto distance = pointNomal.getLength();


	Point finalP = soldierpoint ;
	finalP.y += sin(atan2(vr.x,vr.y) - M_PI / 2) * 108 * getBuildingArmature()->getScale();
	finalP.x -= cos(atan2(vr.x,vr.y) - M_PI / 2) * 108 * getBuildingArmature()->getScale();
	//ÒÆ¶¯
	auto action = Sequence::createWithTwoActions(MoveTo::create( getFlyTime(distance),finalP),
		CallFuncN::create(CC_CALLBACK_1(UIPiercingCannon::flyEndCallBack,this))
		);
	fly->runAction(action);

}

void UIBunkers::flyEndCallBack( Node* node )
{
	  node->removeFromParent();
	  Point finalp = node->getPosition();
	  finalp.y -= sin(node->getRotation() / 180 * M_PI) * 108 * getBuildingArmature()->getScale();
      finalp.x += cos(node->getRotation() / 180 * M_PI) * 108 * getBuildingArmature()->getScale();
	  doBoom(finalp);

}

void UIBunkers::updateFlyWays( float dt )
{
	auto target = getTarget();
	if (!target || _flys.empty())
	{
		return;
	}
	auto tarpos = MapHelper::getPointFromCoord(target->getCoord().x,target->getCoord().y); 
	for (auto item : _flys)
	{
		if (item->getParent() != nullptr)
		{   
			auto flypos =  item->getPosition();
			flypos.y -= sin(item->getRotation() / M_PI * 180) * 108 * getBuildingArmature()->getScale();
			flypos.x += cos(item->getRotation() / M_PI * 180) * 108 * getBuildingArmature()->getScale();
			auto dis = (tarpos - flypos).getLength();
			if (isTouched(dis))
			{
				item->stopAllActions();
				item->removeFromParent();
				doBoom(flypos);
			}
		}
	}
}
