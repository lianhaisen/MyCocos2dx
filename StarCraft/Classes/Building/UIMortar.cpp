#include "UIMortar.h"
#include "BuildingCommon.h"
#include "BuildingConfig.h"

void ParabolyTo::update( float time )
{
  if (!_target)
  {   

	  return;
  }
   float posx = _currentPos.x;
   float posy = _currentPos.y;
   if (_offsetX != 0)
   {
	   posx += _offsetX;
	   posy =  getPosY(posx);
   }
   else
   {
	   posy += _offsetY; 
   }
   Point nextP =  Point(posx,posy);

   auto pointNomal = nextP - _currentPos; 	
   auto vr = pointNomal.getNormalized();
   //_target->setRotation(atan2(vr.x,vr.y) * 180 / M_PI  - 90);
   _target->setPosition(posx,posy);


  _currentPos = nextP;
 

}

ParabolyTo * ParabolyTo::create( float t,const ParabolyCfg cfg )
{
	 auto p = new ParabolyTo();
	 p->init(t,cfg);
	 p->autorelease();
	 return p;
}

bool ParabolyTo::init( float t,ParabolyCfg cfg )
{   
	if (ActionInterval::initWithDuration(t))
	{   
		_config = cfg;
		_currentPos = cfg.shootPos;
		auto tmp = cfg.endPos - cfg.shootPos;
		if (abs(cfg.endPos.x - cfg.shootPos.x) > 1.5f && abs(cfg.beginPos.x - cfg.shootPos.x) > 1.5f)
		{
		   _offsetX = tmp.x / (t / Director::getInstance()->getAnimationInterval());
		}
		_offsetY = tmp.y / (t / Director::getInstance()->getAnimationInterval());
		return true;
	}
	return false;
}

ActionInterval* ParabolyTo::reverse() const 
{
	return nullptr;
}

ActionInterval * ParabolyTo::clone() const 
{
	return nullptr;
}

float ParabolyTo::getPosY( float posX )
{   
	Point A = _config.beginPos;
	Point B = _config.shootPos;
	Point C = _config.endPos;
	float X = posX;
	float tmp = _currentPos.y;
	tmp =  (C.y * (X - A.x) * (X - B.x) ) / ( (C.x - A.x) * ( C.x - B.x));
	tmp += (B.y * (X - A.x) * ( X - C.x)) / ( (B.x - A.x) * (B.x - C.x));
	tmp += (A.y * (X - B.x) * (X - C.x)) / ( (A.x - B.x)  * (A.x - C.x));
	return tmp;

}

ParabolyTo::ParabolyTo()
{
	_offsetX = 0;
	_offsetY = 0;
}


void UIMortar::doFly()
{
    auto fly = getIdleFly();
	auto bone = getFortBuilding()->getBone("point");
	auto towerpoint   = MapHelper::getPointFromCoord(getCoord().x,getCoord().y);
	towerpoint.x +=  bone->getWorldInfo()->x * getBuildingArmature()->getScale();
	towerpoint.y +=  bone->getWorldInfo()->y * getBuildingArmature()->getScale();


	auto bone2    = getFortBuilding()->getBone("point2");
	auto beginpos =  MapHelper::getPointFromCoord(getCoord().x,getCoord().y);
	beginpos.x +=  bone2->getWorldInfo()->x * getBuildingArmature()->getScale();
	beginpos.y +=  bone2->getWorldInfo()->y * getBuildingArmature()->getScale();

	auto soldierpoint = getTmpPt();
	auto pointNomal = soldierpoint - towerpoint;
	auto vr = pointNomal.getNormalized();
	

	getPlaceLayer()->addChild(fly,(MAX_GRID_COUNT + 1) * (MAX_GRID_COUNT + 1));
	fly->setPosition(towerpoint);

	auto flybone = fly->getBone("flytail");
	if (flybone)
	{
		auto p1 = CCParticleSystemQuad::create("building/710003/710003_smoke.plist");
		flybone->addDisplay(p1, 0);
		flybone->changeDisplayWithIndex(0, true);
		flybone->setLocalZOrder(100);
	}

	ParabolyCfg cfg;
	cfg.shootPos = towerpoint ;
	cfg.endPos   = soldierpoint;
	cfg.beginPos = beginpos;
	                
	auto action = Sequence::createWithTwoActions(
		                    ParabolyTo::create(getFlyTime(pointNomal.getLength()),cfg),
							CallFuncN::create(CC_CALLBACK_1(UIPiercingCannon::flyEndCallBack,this))
		                  );      
	fly->runAction(action);
	fly->getAnimation()->play(BuildingCommon::getFlystring(getLevel()),-1,1);




}






