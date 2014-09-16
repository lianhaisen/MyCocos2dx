#include "UILightingTower.h"
#include "BuildingCommon.h"
#include "BuildingConfig.h"
#include "Soldier.h"
#include "SoldierConfig.h"
#include "BattleData.h"
#include "AppMacros.h"


UILightingTower::UILightingTower()
{
	_flyArmature = nullptr;
	_boomArmature = nullptr;
}

UILightingTower::~UILightingTower()
{
	CC_SAFE_RELEASE(_flyArmature);
	CC_SAFE_RELEASE(_boomArmature);
	this->unschedule(schedule_selector(UILightingTower::updateLogic));
	this->unschedule(schedule_selector(UILightingTower::updateAttack));
}

void UILightingTower::updateStatus()
{
	getBuildingArmature()->getAnimation()->play(BuildingCommon::getTowerBodyStringFromLevel(getLevel()));
}

BattleLogic* UILightingTower::findTarget()
{
	for (auto item: _watchilist)
	{
		if (!item->soldierlist.empty())
		{   
			return item->soldierlist.front();
			break;
		}
	}
	return nullptr;
}


void UILightingTower::animationEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	if (movementType != COMPLETE)
	{
		return;
	}   

		if (movementID.compare(BuildingCommon::getAttackstring(getLevel()).c_str()) == 0)
		{    
			doFly();
			return;
		}

		//飞行
		if (movementID.compare(BuildingCommon::getFlystring(getLevel()).c_str()) == 0)
		{
			getFlyArmature()->removeFromParent();
			doBoom();
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


void UILightingTower::updateAttack(float dt)
{
	if (this->getStatus() == e_battle_status_attack )
	{
		auto target = getTarget();
		if (target == nullptr || target->getNewCoord() == nullptr || !isTargetInFightArea(target->getNewCoord()) )
		{   
			setStatus(e_battle_status_idle);
		}
		else
		{   
			doAttack();
		}
	}
	
}

void UILightingTower::initWatchlist()
{   

	if (!_watchilist.empty())
	{
		return;
	}
	//暂时先不用min
	auto cfg    =  BuildingConfig::getBuildingConfig(getType());
	auto mindis = pow(cfg->min_range,2);
	auto maxdis = pow(cfg->max_range,2);
	auto range  = cfg->max_range; 
    
	auto midcoord = getMidCoord();
	
	//TODO 暂时没有按照螺旋形态加入
	for(int i = 0 ; i <= range ; i++)
	{
		for (int j = 0 ; j <= range; j++)
		{   
			auto dis = pow(i,2) + pow(j,2);
			if (dis >= mindis && dis <= maxdis)
			{
				//第一象限
				if (MapHelper::checkIsInPlace(midcoord.x + i,midcoord.y + j,0))
				{
					_watchilist.push_back(BattleData::getInstance()->getSoldierCoord(Coord(midcoord.x + i,midcoord.y + j)));
				}

				// 第二象限
				if (MapHelper::checkIsInPlace(midcoord.x - i,midcoord.y + j,0))
				{
					_watchilist.push_back(BattleData::getInstance()->getSoldierCoord(Coord(midcoord.x - i,midcoord.y + j)));
				}

				// 第三象限

				if (MapHelper::checkIsInPlace(midcoord.x - i,midcoord.y - j,0))
				{
					_watchilist.push_back(BattleData::getInstance()->getSoldierCoord(Coord(midcoord.x - i,midcoord.y - j)));
				}

				//第四象限

				if (MapHelper::checkIsInPlace(midcoord.x + i,midcoord.y - j,0))
				{
					_watchilist.push_back(BattleData::getInstance()->getSoldierCoord(Coord(midcoord.x + i,midcoord.y - j)));
				}
			}
		}
	}

}

bool UILightingTower::isTargetInFightArea( NewCoord * target )
{
	auto it = std::find(_watchilist.begin(),_watchilist.end(),target);
	if (it != _watchilist.end())
	{
		return true;
	}
	return false;
}

void UILightingTower::updateLogic( float dt )
{
	if (getStatus() ==  e_battle_status_dead || getHp() == 0)
	{   
		//死亡 退出
		this->unschedule(schedule_selector(UILightingTower::updateLogic));
		this->unschedule(schedule_selector(UILightingTower::updateAttack));
		return;
	}

	//空闲状态
	if (getStatus() == e_battle_status_idle)
	{
		auto target = getTarget();
		if (target == nullptr || target->getStatus() == e_battle_status_dead ||target->getNewCoord() == nullptr || !isTargetInFightArea(target->getNewCoord()) )
		{   
			//重新找一个目标
			setTarget(findTarget());
			return;
		}
		else
		{
			//可以攻击了
			setStatus(e_battle_status_attack);
		}
	}

}


bool UILightingTower::initWithBattleLogic( DBuilding * db )
{
	auto item = Building::initWithBattleLogic(db);
	setTarget(nullptr);

	initWatchlist();
	this->schedule(schedule_selector(UILightingTower::updateLogic),SCHEDULE_INTERVAL);
	auto cfg = BuildingConfig::getBuildingConfig(this->getType());


	this->schedule(schedule_selector(UILightingTower::updateAttack),float(cfg->attack_speed) / 1000);
	getBuildingArmature()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(UILightingTower::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	return item;
}

void UILightingTower::doAttack()
{   
	auto target = getTarget();
	if (target && target->getStatus() != e_battle_status_dead)
	{   
		auto soldierpoint = MapHelper::getPointFromCoord(target->getCoord().x,target->getCoord().y);
		setTmpPt(soldierpoint);
		getBuildingArmature()->getAnimation()->play(BuildingCommon::getAttackstring(getLevel()),-1,0);
	}

}

void UILightingTower::doFly()
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
   
	auto bone = getBuildingArmature()->getBone("head");
	auto towerpoint   = MapHelper::getPointFromCoord(getCoord().x,getCoord().y);
	towerpoint.x +=  bone->getWorldInfo()->x * getBuildingArmature()->getScale();
	towerpoint.y +=  bone->getWorldInfo()->y * getBuildingArmature()->getScale();
	
	getPlaceLayer()->addChild(getFlyArmature(),(MAX_GRID_COUNT + 1) * (MAX_GRID_COUNT + 1));
	getFlyArmature()->setPosition(towerpoint);
	
	auto soldierpoint = getTmpPt();
	auto pointNomal = soldierpoint - towerpoint; 	
	auto vr = pointNomal.getNormalized();
	getFlyArmature()->setRotation(atan2(vr.x,vr.y) * 180 / M_PI  - 90);

	//TODO 动态获得动画长度 
	auto distance = pointNomal.getLength();
	getFlyArmature()->setScaleX(distance / 700 );		
	getFlyArmature()->getAnimation()->play(BuildingCommon::getFlystring(getLevel()),-1,0);

}

void UILightingTower::doBoom()
{
	if (getBoomArmature() == nullptr)
	{   
		setBoomArmature(Armature::create(BuildingCommon::getArmatureNameFromType(_type).c_str()));
		getBoomArmature()->setScale(getBuildingArmature()->getScale());
		getBoomArmature()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(UILightingTower::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	if (getBoomArmature()->getParent() != nullptr)
	{
		getBoomArmature()->removeFromParent();
	}

	getPlaceLayer()->addChild(getBoomArmature(),(MAX_GRID_COUNT + 1) * (MAX_GRID_COUNT + 1));
	getBoomArmature()->setPosition(getTmpPt());
    getBoomArmature()->getAnimation()->play(BuildingCommon::getBoomstring(getLevel()),-1,0);	
}

Node * UILightingTower::getPlaceLayer()
{
	return this->getParent();
}

Point UILightingTower::getTmpPt()
{   
	auto target = getTarget();
	if (target && target->getStatus() != e_battle_status_dead)
	{
	    _tempPoint = MapHelper::getPointFromCoord(target->getCoord().x,target->getCoord().y);
	}
	return _tempPoint;
}

bool UILightingTower::isTouched( float dis )
{
	return  dis <=  ((UNIT_HEIGHT + UNIT_WIDTH) / 2)  * 2;
}

bool UILightingTower::isInAoeHurt( float dis )
{
	int attackrange = 2;
	auto cfg = BuildingConfig::getBuildingConfig(getType())->aoe_range;
	if (cfg > attackrange)
	{
		attackrange = cfg;
	}
	return  dis <=  ((UNIT_HEIGHT + UNIT_WIDTH) / 2) * attackrange;
}

float UILightingTower::getFlyTime( float distance )
{
	float count = distance / ((UNIT_HEIGHT + UNIT_WIDTH) / 2);
	auto time = count / BuildingConfig::getBuildingConfig(getType())->fly_speed;
	return time;	
}

