#include "UIBlockingTower.h"
#include "BuildingCommon.h"
#include "BuildingConfig.h"


void UIBlockingTower::doFly()
{
	if (getFlyArmature() == nullptr)
	{
		setFlyArmature(Armature::create(BuildingCommon::getArmatureNameFromType(_type).c_str()));
		getFlyArmature()->setScale(getBuildingArmature()->getScale());
		getFlyArmature()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(UILightingTower::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	if (getFlyArmature()->getParent() != nullptr)
	{
		getFlyArmature()->removeFromParent();
	}

	auto bone = getBuildingArmature()->getBone(getHeadStringFromId(_headId));
	auto towerpoint   = MapHelper::getPointFromCoord(getCoord().x,getCoord().y);
	towerpoint.x +=  bone->getWorldInfo()->x * getBuildingArmature()->getScale();
	towerpoint.y +=  bone->getWorldInfo()->y * getBuildingArmature()->getScale();
	auto soldierpoint = getTmpPt();
	auto pointNomal = soldierpoint - towerpoint;

	getPlaceLayer()->addChild(getFlyArmature(),(MAX_GRID_COUNT + 1) * (MAX_GRID_COUNT + 1));
	getFlyArmature()->setPosition(towerpoint);

	auto vr = pointNomal.getNormalized();
	getFlyArmature()->setRotation(atan2(vr.x,vr.y) * 180 / M_PI  - 90);


	auto distance = pointNomal.getLength();
	getFlyArmature()->setScaleX(distance / 740);

	getFlyArmature()->getAnimation()->play(BuildingCommon::getFlystring(getLevel()),-1,0);
	
}

void UIBlockingTower::getHeadIdByTargetCoord( Coord co )
{
	auto selfpoint =  MapHelper::getPointFromCoord(getMidCoord().x ,getMidCoord().y);
	auto targetpoint = MapHelper::getPointFromCoord(co.x,co.y);

	auto vr = (targetpoint - selfpoint).getNormalized();
	auto degree = int(atan2(vr.x,vr.y) * 180 / M_PI) ;
	
    
	if (degree < 0)
	{
		degree += 360;
	}
	if (degree >= 360)
	{
		degree %= 360;
	}
	int tempindex = degree / 45 + 1;
	int left = degree % 45;
	if (left > 45 / 2 )
	{
		tempindex += 1;
	}
	if (tempindex > 8)
	{
		tempindex -= 8;
	}
	_headId = tempindex;
	
}

void UIBlockingTower::doAttack()
{
	if (getAttackArmature() == nullptr)
	{
		setAttackArmature(Armature::create(BuildingCommon::getArmatureNameFromType(_type).c_str()));
		getAttackArmature()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(UIBlockingTower::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	if (getAttackArmature()->getParent() != nullptr)
	{
		getAttackArmature()->removeFromParent();
	}
	auto target = getTarget();
	if (target && target->getStatus() != e_battle_status_dead)
	{   
		setTmpPt(MapHelper::getPointFromCoord(target->getCoord().x,target->getCoord().y));
		getHeadIdByTargetCoord(target->getCoord());
		auto bone = getBuildingArmature()->getBone(getHeadStringFromId(_headId));
		bone->addDisplay(getAttackArmature(),0);
		bone->changeDisplayWithIndex(0,true);
		getAttackArmature()->getAnimation()->play(BuildingCommon::getAttackstring(getLevel()),-1,0);
	}
}

UIBlockingTower::UIBlockingTower()
{   
	_attackArmature = nullptr;
	_headId = 1;
}

std::string UIBlockingTower::getHeadStringFromId( int id )
{
	auto str = String::createWithFormat("head%02d",id)->getCString();
	return str;
}

UIBlockingTower::~UIBlockingTower()
{
	CC_SAFE_RELEASE(_attackArmature);
}
