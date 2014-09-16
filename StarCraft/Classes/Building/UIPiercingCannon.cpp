#include "UIPiercingCannon.h"
#include "BuildingCommon.h"
#include "AppMacros.h"
#include "BuildingConfig.h"

UIPiercingCannon::UIPiercingCannon()
{

}

UIPiercingCannon::~UIPiercingCannon()
{
	_flys.clear();
	_booms.clear();
	this->unschedule(schedule_selector(UIPiercingCannon::updateFlyWays));
}

Armature * UIPiercingCannon::getIdleFly()
{
	Armature * result = nullptr;
	for (auto item : _flys)
	{
		if (item->getParent() == nullptr)
		{
			result = item;
			break;
		}
	}
	if (result == nullptr)
	{
		result = Armature::create(BuildingCommon::getArmatureNameFromType(_type));
		result->setScale(getBuildingArmature()->getScale());
		_flys.pushBack(result);
	}
	return result;
}

Armature * UIPiercingCannon::getIdleBoom()
{
	Armature * result = nullptr;
	for (auto item : _booms)
	{
		if (item->getParent() == nullptr)
		{
			result = item;
			break;
		}
	}
	if (result == nullptr)
	{
		result = Armature::create(BuildingCommon::getArmatureNameFromType(_type));
		result->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(UIPiercingCannon::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		result->setScale(getBuildingArmature()->getScale());
		_booms.pushBack(result);
	}
	return result;
}

void UIPiercingCannon::animationEvent( cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID )
{
	//转头
	if (movementType != COMPLETE)
	{
		return;
	}

	auto strva = Value(movementID.substr(movementID.length() - 3,3).c_str());
	if (strva.asString().compare("nor") == 0)
	{
		auto va = Value(movementID.substr(movementID.length() - 6,2).c_str());
		_currentIndex = va.asInt();
		if (_currentIndex != _degreeIndex)
		{   
			getFortBuilding()->getAnimation()->play(BuildingCommon::getNomalTowerStringFromLevel(getLevel(),getMovement(_currentIndex,_degreeIndex)),-1,0);
		}
		else
		{   
			auto target = getTarget();
			if (target && target->getStatus() != e_battle_status_dead)
			{
				getFortBuilding()->getAnimation()->play(BuildingCommon::getTowerStringFromLevel(getLevel(),_degreeIndex),-1,0);
			}

		}
		return;
	}

	if (movementID.compare(BuildingCommon::getBoomstring(getLevel()).c_str()) == 0)
	{
	
		//TODO检查AOE伤害
		if (this->getStatus() == e_battle_status_attack)
		{
			auto target = getTarget();
			if (target)
			{
				auto boompos = armature->getPosition();
				auto targetpos = MapHelper::getPointFromCoord(target->getCoord().x,target->getCoord().y);
				auto dis = (boompos - targetpos).getLength();
				if (isInAoeHurt(dis))
				{
					bool isDead  = target->subHp(-BuildingConfig::getBuildingConfig(getType())->attack);
					if (isDead)
					{
						setStatus(e_battle_status_idle);
						getTarget()->dead();
					}
					else
					{
						setStatus(e_battle_status_attack);
					}
				}
			}		
		}
		armature->removeFromParent();
		return;
	}
}

void UIPiercingCannon::frameEvent( Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex )
{
	if (frameEventName.compare("attack")  == 0)
	{
		doFly();
		return;
	}
}

void UIPiercingCannon::doFly()
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
	//移动
	auto action = Sequence::createWithTwoActions(MoveTo::create( getFlyTime(distance),soldierpoint),
			                                        CallFuncN::create(CC_CALLBACK_1(UIPiercingCannon::flyEndCallBack,this))
			                                    );
	fly->runAction(action);
       
	

}

void UIPiercingCannon::flyEndCallBack( Node* node)
{
    node->removeFromParent();
	doBoom(node->getPosition());
}

void UIPiercingCannon::doBoom( Point pt )
{
	auto boom = getIdleBoom();
	boom->setPosition(pt);
	getPlaceLayer()->addChild(boom);
	boom->getAnimation()->play(BuildingCommon::getBoomstring(getLevel()),-1,0);
}



bool UIPiercingCannon::initWithBattleLogic( DBuilding * db )
{
	 auto item =  UILaserCannon::initWithBattleLogic(db);
	 this->schedule(schedule_selector(UIPiercingCannon::updateFlyWays),0.1f);
	 return item;
}

void UIPiercingCannon::updateFlyWays( float dt )
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


