#include "UILaserCannon.h"
#include "BuildingCommon.h"
#include "BuildingConfig.h"

void UILaserCannon::updateStatus()
{  
	if (getBuildingArmature() != nullptr)
	{
		getBuildingArmature()->getAnimation()->play(BuildingCommon::getTowerBodyStringFromLevel(getLevel()));
		if(!getFortBuilding())
		{
		   CreateFordBuilding();
		}
		auto bone = getBuildingArmature()->getBone("head");
		bone->addDisplay(getFortBuilding(),0);
		bone->changeDisplayWithIndex(0,true);
		getFortBuilding()->getAnimation()->play(BuildingCommon::getNomalTowerStringFromLevel(getLevel(),_currentIndex),-1,0);
	}
}

UILaserCannon::UILaserCannon()
{
	_Fort = nullptr;
	_degreeIndex = 1;
	_currentIndex = 1;
}

UILaserCannon::~UILaserCannon()
{
	CC_SAFE_RELEASE(_Fort);
}

void UILaserCannon::CreateFordBuilding()
{
	setFortBuilding(CCArmature::create(BuildingCommon::getArmatureNameFromType(getBType())));
	getFortBuilding()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(UILaserCannon::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	getFortBuilding()->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(UILaserCannon::frameEvent,this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

void UILaserCannon::animationEvent( cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID )
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
				getFortBuilding()->getAnimation()->play(BuildingCommon::getTowerStringFromLevel(getLevel(),_degreeIndex));
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
			if (target )
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

int UILaserCannon::getMovement( int from, int to )
{   
    int resutl = from;
	//判定顺时针,逆时针
	if (from != to)
	{   
		int degreefrom = (from - 1) * 15;
		degreefrom = degreefrom <= 180 ? degreefrom : degreefrom - 360; 
		int degreeto   = (to - 1) * 15;
		degreeto = degreeto <= 180 ? degreeto : degreeto -360;
		int ss = degreeto - degreefrom;
		if (abs(ss) > 180)
		{
			ss = -ss;
		}
		if (ss > 0)
		{
			resutl += 1;
		}
		else
		{
			resutl -= 1;
		}
		if (resutl > 24)
		{
			resutl = 1;
		}
		if (resutl <= 0)
		{
			resutl = 24;
		}
	}
	return resutl;
}


void UILaserCannon::doAttack()
{   
	auto target = getTarget();
	if (target && target->getStatus() != e_battle_status_dead)
	{   
		setTmpPt(MapHelper::getPointFromCoord(target->getCoord().x,target->getCoord().y));
		getHeadIdByTargetCoord(target->getCoord());
		getFortBuilding()->getAnimation()->play(BuildingCommon::getNomalTowerStringFromLevel(getLevel(),getMovement(_currentIndex,_degreeIndex)),-1,0);
	}

}

void UILaserCannon::doFly()
{
	if (getFlyArmature() == nullptr)
	{
		setFlyArmature(Armature::create(BuildingCommon::getArmatureNameFromType(_type).c_str()));
		getFlyArmature()->setScale(getBuildingArmature()->getScale());
		getFlyArmature()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(UILaserCannon::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		getFlyArmature()->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(UILaserCannon::frameEvent,this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	}

	if (getFlyArmature()->getParent() != nullptr)
	{
		getFlyArmature()->removeFromParent();
	}

	auto bone = getFortBuilding()->getBone("point");
	auto towerpoint   = MapHelper::getPointFromCoord(getCoord().x,getCoord().y);
	towerpoint.x +=  bone->getWorldInfo()->x * getBuildingArmature()->getScale();
	towerpoint.y +=  bone->getWorldInfo()->y * getBuildingArmature()->getScale();
	auto soldierpoint = getTmpPt();
	auto pointNomal = soldierpoint - towerpoint;
	auto vr = pointNomal.getNormalized();
	getFlyArmature()->setRotation(atan2(vr.x,vr.y) * 180 / M_PI  - 90);

	getPlaceLayer()->addChild(getFlyArmature(),(MAX_GRID_COUNT + 1) * (MAX_GRID_COUNT + 1));
	getFlyArmature()->setPosition(towerpoint);

	//TODO 动态获得动画长度 
	auto distance = pointNomal.getLength();
	getFlyArmature()->setScaleX(distance / 630);


	getFlyArmature()->getAnimation()->play(BuildingCommon::getFlystring(getLevel()),-1,0);
	


}

void UILaserCannon::getHeadIdByTargetCoord( Coord co )
{
	auto selfpoint =  MapHelper::getPointFromCoord(getMidCoord().x,getMidCoord().y);
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
	int tempindex = degree / 15 + 1;
	int left = degree % 15;
	if (left > 15 / 2 )
	{
		tempindex += 1;
	}
	if (tempindex > 24)
	{
		tempindex -= 24;
	}
	_degreeIndex = tempindex;

}

void UILaserCannon::frameEvent( Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex )
{
	if (frameEventName.compare("attack")  == 0)
	{
		doFly();
		return;
	}
	if (frameEventName.compare("boom") == 0)
	{
		getFlyArmature()->removeFromParent();
		doBoom();
		return;
	}
}




