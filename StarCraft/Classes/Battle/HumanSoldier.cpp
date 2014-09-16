#include "HumanSoldier.h"
#include "AppMacros.h"
#include "AStar.h"
#include "BattleScene.h"
#include "BattleData.h"
#include "SoldierData.h"
#include "SoldierConfig.h"
#include "BuildingConfig.h"
#include "BuildingCommon.h"

//机枪兵700001
SoldierJqb::SoldierJqb( int id ) : Soldier(id)
{
	//_fly = NULL;
	//_boom = NULL;
}

SoldierJqb::~SoldierJqb( void )
{
	//_fly->removeFromParent();
	//_fly->release();
	//_boom->removeFromParent();
	//_boom->release();
}

void SoldierJqb::doFly()
{
	auto target = getTarget();
	if (target && target->getStatus() != e_battle_status_dead)
	{
		auto fly = getIdleFly();
		auto point1 = MapHelper::getPointFromCoord(getMidCoord().x,getMidCoord().y);
		auto bone = getBody()->getBone("point");
		point1.x += bone->getWorldInfo()->x * getBody()->getParent()->getScale() * getBody()->getScaleX();
		point1.y += bone->getWorldInfo()->y * getBody()->getParent()->getScale();

		auto cfg = BuildingConfig::getBuildingConfig(target->getType());
		int randX = (rand()%2?-1:1)*(rand()%(10*cfg->grid));
		int randY = (rand()%2?-1:1)*(rand()%(10*cfg->grid));
		auto point2 = MapHelper::getPointFromCoord(target->getMidCoord().x, target->getMidCoord().y);
		point2.x += randX;
		point2.y += randY;

		auto vr = ( point2 - point1).getNormalized();
		fly->setRotation(atan2(vr.x,vr.y) * 180 / M_PI  - 90);
		fly->setPosition(point1);

		BattleScene::getPlaceLayer()->addChild(fly);
		//BuildingCommon::setTmpPoint(point1,point2);
		fly->getAnimation()->play("01_fly01", -1, 1);

		float dis = point1.getDistance(point2)/((UNIT_WIDTH+UNIT_HEIGHT)/2);
		float speed = 1/SoldierConfig::getSoldierConfig(getType())->fly_speed;
		float t = dis*speed/20;
		auto move = MoveTo::create(t, point2);
		auto action = Sequence::create(move, CallFuncN::create(CC_CALLBACK_1(SoldierJqb::doBoom,this)), NULL);
		fly->runAction(action);
	}
}

void SoldierJqb::doBoom(Node* ref)
{
	auto point = ref->getPosition();
	ref->removeFromParent();
	auto target = getTarget();
	if (target && target->getStatus() != e_battle_status_dead)
	{
		auto boom = getIdleBoom();
		boom->setPosition(point);
		BattleScene::getPlaceLayer()->addChild(boom);
		boom->getAnimation()->play("01_boom01", -1, 0);
	}
}

void SoldierJqb::animationEvent( Armature *armature, MovementEventType movementType, const string& movementID )
{
	if (movementType == COMPLETE)
	{
		string temp = movementID;
		temp = temp.substr(0, temp.length()-2);
		char strAttack[15];
		sprintf(strAttack, "%02d_attack", 1);
		char strBoom[15];
		sprintf(strBoom, "%02d_boom", 1);

		auto target = getTarget();
		if (temp.compare(strAttack) == 0)
		{
			//doFly();
		}
		else if (temp.compare(strBoom) == 0)
		{
			armature->removeFromParent();
			bool isDead  = getTarget()->subHp(-SoldierConfig::getSoldierConfig(getType())->attack);
			if (isDead)
			{
				getTarget()->dead();
				setStatus(e_battle_status_idle);
				m_direction = 0;
				unschedule(schedule_selector(Soldier::updateAttack));
			} 
		}
	}
}

void SoldierJqb::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{  
	if(strcmp(evt.c_str(),"attack") == 0)  
	{  
		doFly();
	}  	
}  

//炮兵700002
SoldierPb::SoldierPb( int id ) : Soldier(id)
{
	//_fly = NULL;
	//_boom = NULL;
}

SoldierPb::~SoldierPb( void )
{
	//_fly->removeFromParent();
	//_fly->release();
	//_boom->removeFromParent();
	//_boom->release();
}

void SoldierPb::doFly()
{
	auto target = getTarget();
	if (target && target->getStatus() != e_battle_status_dead)
	{
		auto fly = getIdleFly();
		auto point1 = MapHelper::getPointFromCoord(getMidCoord().x,getMidCoord().y);
		auto bone = getBody()->getBone("point");
		point1.x += bone->getWorldInfo()->x * getBody()->getParent()->getScale() * getBody()->getScaleX();
		point1.y += bone->getWorldInfo()->y * getBody()->getParent()->getScale();

		auto cfg = BuildingConfig::getBuildingConfig(target->getType());
		int randX = (rand()%2?-1:1)*(rand()%(10*cfg->grid));
		int randY = (rand()%2?-1:1)*(rand()%(10*cfg->grid));
		auto point2 = MapHelper::getPointFromCoord(target->getMidCoord().x,target->getMidCoord().y);
		point2.x += randX;
		point2.y += randY;

		auto vr = ( point2 - point1).getNormalized();
		fly->setRotation(atan2(vr.x,vr.y) * 180 / M_PI  - 90);
		fly->setPosition(point1);

		BattleScene::getPlaceLayer()->addChild(fly);
		//BuildingCommon::setTmpPoint(point1,point2);
		fly->getAnimation()->play("01_fly01", -1, 1);

		float dis = point1.getDistance(point2)/((UNIT_WIDTH+UNIT_HEIGHT)/2);
		float speed = 1/SoldierConfig::getSoldierConfig(getType())->fly_speed;
		float t = dis*speed/20;
		auto move = MoveTo::create(t, point2);
		auto action = Sequence::create(move, CallFuncN::create(CC_CALLBACK_1(SoldierPb::doBoom,this)), NULL);
		fly->runAction(action);
	}
}

void SoldierPb::doBoom( Node* ref )
{
	auto point = ref->getPosition();
	ref->removeFromParent();
	auto target = getTarget();
	if (target && target->getStatus() != e_battle_status_dead)
	{
		auto boom = getIdleBoom();
		boom->setPosition(point);
		BattleScene::getPlaceLayer()->addChild(boom);
		boom->getAnimation()->play("01_boom01", -1, 0);
	}
}

//void SoldierPb::flyMove(Coord coord)
//{
//	Point origin = _fly->getPosition();
//	Point end = MapHelper::convertCoordToPoint(coord);
//
//	float a = 9.8f;
//	float s = sqrt(pow((origin.x - end.x), 2) + pow((origin.y - end.y), 2));
//	float v = 100;
//	////float t = 1/m_csoldier->fly_speed;
//	float t1 = s / v;
//	float vy = 10;//(s + 0.5*a*t1*t1) / t1;
//
//	int moveCount = 10;
//	float t0 = t1/moveCount;
//	Point p = origin;
//
//	
//	Vector<FiniteTimeAction*> moves;
//	for (int i=0; i<moveCount; i++)
//	{
//		if (origin.x > end.x)
//		{
//			p.x -= v*t0;
//		} 
//		else
//		{
//			p.x += v*t0;
//		}
//
//		float tt = t0 * (i+1);
//		if (origin.y > end.y)
//		{
//			p.y = origin.y - (0.5*a*t1*tt-0.5*a*tt*tt);
//		} 
//		else
//		{
//			p.y = origin.y + (0.5*a*t1*tt-0.5*a*tt*tt);
//		}
//
//		log("p.y = %f", p.y);
//		MoveTo *move = MoveTo::create(t0, p);
//		moves.pushBack(move);
//	}
//	log("origin.y = %f", origin.y);
//	log("end.y = %f", end.y);
//	_fly->runAction(Sequence::create(moves));
//}
//
//void SoldierPb::runBoomAction()
//{
//	if (_fly != NULL)
//	{
//		_fly->stopAllActions();
//		_fly->setVisible(false);
//	}
//
//	auto parent = BattleScene::getPlaceLayer();
//	auto target = getTarget();
//	if (_boom == NULL)
//	{
//		auto sd = SoldierData::getSoldierData(getId());
//		int type = sd->type / 100;
//		char sType[10];
//		sprintf(sType,"%d",type);
//
//		_boom = Armature::create(sType);
//		_boom->setScale(0.5); 
//		parent->addChild(_boom);
//		_boom->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Soldier::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//	} 
//	_boom->setVisible(true);
//	_boom->setPosition(MapHelper::convertCoordToPoint(target->getMidCoord()));
//	_boom->getAnimation()->play("01_boom01", -1, 0);
//
//
//}

void SoldierPb::animationEvent( Armature *armature, MovementEventType movementType, const std::string& movementID )
{
	/*
	if (movementType == LOOP_COMPLETE || movementType == COMPLETE)
	{
		string temp = movementID;
		temp = temp.substr(0, temp.length()-2);
		char strAttack[15];
		sprintf(strAttack, "%02d_attack", 1);
		char strBoom[15];
		sprintf(strBoom, "%02d_boom", 1);

		auto target = getTarget();
		if (temp.compare(strAttack) == 0)
		{
			auto parent = BattleScene::getPlaceLayer();
			if (_fly == NULL)
			{
				auto sd = SoldierData::getSoldierData(getId());
				int type = sd->type / 100;
				char sType[10];
				sprintf(sType,"%d",type);

				_fly = Armature::create(sType);
				_fly->setScale(0.5); 
				parent->addChild(_fly);
			} 
			_fly->setVisible(true);

			//TODO:修正弹道起始位置(还是有偏差)
			Point p = Point(0, 0);
			string temp2 = movementID;
			temp2 = temp2.substr(temp2.length()-2, temp2.length());
			if (temp2.compare("01") == 0)
			{
				p = Point(31, 13);
			} 
			else if (temp2.compare("02") == 0)
			{
				p = Point(52, 55);
			}
			else if (temp2.compare("03") == 0)
			{
				p = Point(42, 80);
			}
			else if (temp2.compare("04") == 0)
			{
				p = Point(0, 15);
			}
			else if (temp2.compare("05") == 0)
			{
				p = Point(17, 88);
			}
			Point p0 = MapHelper::convertCoordToPoint(getMidCoord());
			_fly->setPosition(Point(p0.x + p.x, p0.y + p.y));

			//修正弹道起始角度
			float rotation = 0;
			switch (getDirection())
			{
			case e_battle_dir_east :
				rotation = 335;
				break;
			case e_battle_dir_es :
				rotation = 0;
				break;
			case e_battle_dir_south :
				rotation = 45;
				break;
			case e_battle_dir_ws :
				rotation = 90;
				break;
			case e_battle_dir_west :
				rotation = 135;
				break;
			case e_battle_dir_wn :
				rotation = 180;
				break;
			case e_battle_dir_north :
				rotation = 225;
				break;
			case e_battle_dir_en :
				rotation = 270;
				break;
			default:
				break;
			}
			_fly->setRotation(rotation);
			_fly->getAnimation()->play("01_fly01");

			//TODO:残影，设置方向
			auto sc = SoldierConfig::getSoldierConfig(getType());
			auto move = MoveTo::create(sc->fly_speed, MapHelper::convertCoordToPoint(target->getMidCoord()));
			auto action = Sequence::create(move, CallFunc::create(CC_CALLBACK_0(SoldierPb::runBoomAction,this)), NULL);
			_fly->runAction(action);

			//flyMove(target->getMidCoord());
		}
		else if (temp.compare(strBoom) == 0)
		{
			if (_boom != NULL)
			{
				_boom->stopAllActions();
				_boom->setVisible(false);
			}
			auto sc = SoldierConfig::getSoldierConfig(getType());
			bool isDead  = target->subHp(-sc->attack);
			if (isDead)
			{
				setStatus(e_battle_status_idle);
				m_direction = 0;
				target->dead();
				unschedule(schedule_selector(Soldier::updateAttack));
			} 
		}
	}
	*/

	if (movementType == COMPLETE)
	{
		string temp = movementID;
		temp = temp.substr(0, temp.length()-2);
		char strAttack[15];
		sprintf(strAttack, "%02d_attack", 1);
		char strBoom[15];
		sprintf(strBoom, "%02d_boom", 1);

		auto target = getTarget();
		if (temp.compare(strAttack) == 0)
		{
			doFly();
		}
		else if (temp.compare(strBoom) == 0)
		{
			armature->removeFromParent();
			bool isDead  = getTarget()->subHp(-SoldierConfig::getSoldierConfig(getType())->attack);
			if (isDead)
			{
				getTarget()->dead();
				setStatus(e_battle_status_idle);
				m_direction = 0;
				unschedule(schedule_selector(Soldier::updateAttack));
			} 
		}
	}
}

void SoldierPb::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{  
	//if(strcmp(evt.c_str(),"attack") == 0)  
	//{  
	//	doFly();
	//}  	
}

//护士700003
SoldierHs::SoldierHs( int id ) : Soldier(id)
{

}

SoldierHs::~SoldierHs( void )
{

}

//void SoldierHs::setTarget(int target)
//{
//	//TODO:士兵的目标可以是敌方建筑，士兵，英雄
//	auto soldiers = BattleData::getInstance()->getAttackSoldierInBattle();
//	_target = soldiers.at(target);
//	setStatus(e_battle_status_move);
//}

void SoldierHs::findCandidateTarget( vector<BattleLogic*> &candidate )
{
	//1、只能治疗地面单位（所有地面士兵和英雄）
	//2、优先治疗离自己最近的，有掉血的目标
	//3、有多个距离一样的目标，则优先治疗血最少的
	//4、该目标死亡或满血后才会更换目标。
	auto soldier = SoldierConfig::getSoldierConfig(getType());
	multimap<int, BattleLogic*> tmps1;//掉血的士兵
	multimap<int, BattleLogic*> tmps2;//满血的士兵

	//从己方士兵中筛选目标
	auto soldiers = BattleData::getInstance()->getAttackSoldierInBattle();
	map<int,BattleLogic*>::iterator it1;
	for(it1=soldiers.begin(); it1!=soldiers.end(); it1++)
	{
		auto item = it1->second;
		if (item->getStatus() == E_STATUS_DEAD)
		{
			continue;
		}
		auto config = SoldierConfig::getSoldierConfig(item->getType());
		//只能治疗地面单位（所有地面士兵和英雄）
		if (config->unit_type != e_unit_type_ground)
		{
			continue;
		}
		//过滤满血的士兵
		if (item->isFullHp())
		{
			if (item->getId() == getId() || item->getType()/100 == 700003)
			{
				continue;
			}
			int dis = (item->getMidCoord().x - getMidCoord().x)*(item->getMidCoord().x - getMidCoord().x) + (item->getMidCoord().y - getMidCoord().y)*(item->getMidCoord().y - getMidCoord().y);
			tmps2.insert(pair<int,BattleLogic*>(dis, item));
		}
		else
		{
			int dis = (item->getMidCoord().x - getMidCoord().x)*(item->getMidCoord().x - getMidCoord().x) + (item->getMidCoord().y - getMidCoord().y)*(item->getMidCoord().y - getMidCoord().y);
			tmps1.insert(pair<int,BattleLogic*>(dis, item));
		}
	}

	if (tmps1.empty())
	{
		if (!tmps2.empty())
		{
			multimap<int, BattleLogic*>::iterator it2;
			for(it2=tmps2.begin(); it2!=tmps2.end(); it2++)
			{
				if (candidate.size() >= 3)
				{
					break;
				} 
				else
				{
					candidate.push_back(it2->second);
				}
			}
		}
	} 
	else
	{
		multimap<int, BattleLogic*>::iterator it2;
		for(it2=tmps1.begin(); it2!=tmps1.end(); it2++)
		{
			if (candidate.size() >= 3)
			{
				break;
			} 
			else
			{
				candidate.push_back(it2->second);
			}
		}
	}
}

BattleLogic* SoldierHs::findTargetFromCandidate(vector<BattleLogic*> candidate)
{
	log("findAttackTarget");
	BattleLogic* target = nullptr;
	unsigned int step = 9999;
	int hp = 0;
	vector<Coord> tmpPath;

	MapHelper *map = new MapHelper();
	//auto soldiers = BattleData::getInstance()->getAttackSoldierInBattle();
	for (auto item : candidate)
	{
		//auto soldier = soldiers.at(item);
		AStar *a = new AStar();
		a->setMap(map);
		a->setStartPoint(item->getMidCoord());
		a->setEndPoint(getMidCoord());
		a->startSearch();
		vector<Coord> path;
		a->getPath(path);

		if (path.size() < step)
		{
			hp = item->getHp();
			target = item;
			step = path.size();
			tmpPath = path;
		}
		else if (path.size() == step)
		{
			//有多个距离一样的目标，则优先治疗血最少的
			if (item->getHp() < hp)
			{
				hp = item->getHp();
				target = item;
				step = path.size();
				tmpPath = path;
			}
		}
	}

	setPath(tmpPath);
	return target;
}

void SoldierHs::animationEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	if (movementType == LOOP_COMPLETE || movementType == COMPLETE)
	{
		string temp = movementID;
		temp = temp.substr(0, temp.length()-2);
		char compstr[250];
		sprintf(compstr, "%02d_attack", 1);
		char strBoom[15];
		sprintf(strBoom, "%02d_boom", 1);
		if (temp.compare(compstr) == 0)
		{
			doBoom();
		}
		else if (temp.compare(strBoom) == 0)
		{
			armature->removeFromParent();
			bool isDead  = getTarget()->subHp(SoldierConfig::getSoldierConfig(getType())->attack);
			if (isDead)
			{
				//getTarget()->dead();
				setStatus(e_battle_status_idle);
				m_direction = 0;
				unschedule(schedule_selector(Soldier::updateAttack));
			} 
		}
	}
}

void SoldierHs::doBoom()
{
	//auto point = ref->getPosition();
	auto target = getTarget();
	if (target && target->getStatus() != e_battle_status_dead)
	{
		auto boom = getIdleBoom();
		boom->setPosition(MapHelper::convertCoordToPoint(target->getMidCoord()));
		auto parent = BattleScene::getPlaceLayer();
		parent->addChild(boom);
		boom->getAnimation()->play("01_boom01", -1, 0);
	}
}



//自杀机器人700004
SoldierZsjqr::SoldierZsjqr( int id ) : Soldier(id)
{

}

SoldierZsjqr::~SoldierZsjqr( void )
{

}

void SoldierZsjqr::findCandidateTarget( vector<int> &candidate )
{
	//1、优先攻击距离最近的设施
	//2、没有设施的情况下，攻击距离最近的敌方建筑
	//3、不能攻击敌方士兵
	auto soldier = SoldierConfig::getSoldierConfig(getType());
	multimap<int, int> tmps;

	auto bds = BattleData::getInstance()->getDefendBuilding();
	map<int,BattleLogic*>::iterator it;
	for(it=bds.begin(); it!=bds.end(); it++)
	{
		auto bd = it->second;
		if (bd->getStatus() == E_STATUS_DEAD)
		{
			continue;
		}
		auto config = BuildingConfig::getBuildingConfig(bd->getType());
		if (config->building_type == e_preference_facility)
		{
			int dis = (bd->getMidCoord().x - getMidCoord().x)*(bd->getMidCoord().x - getMidCoord().x) + (bd->getMidCoord().y - getMidCoord().y)*(bd->getMidCoord().y - getMidCoord().y);
			tmps.insert(pair<int,int>(dis, bd->getId()));
		}
	}

	//没有设施
	if (tmps.empty())
	{
		map<int,BattleLogic*>::iterator it1;
		for(it1=bds.begin(); it1!=bds.end(); it1++)
		{
			auto bd = it1->second;
			auto config = BuildingConfig::getBuildingConfig(bd->getType());
			//过滤不能成为攻击目标的建筑物，如已被摧毁的建筑，陷阱，障碍物
			if (bd->getStatus() == E_STATUS_DEAD || config->building_type == e_preference_trap
				|| config->building_type == e_preference_barrier1 || config->building_type == e_preference_barrier2)
			{
				continue;
			}

			int dis = (bd->getMidCoord().x - getMidCoord().x)*(bd->getMidCoord().x - getMidCoord().x) + (bd->getMidCoord().y - getMidCoord().y)*(bd->getMidCoord().y - getMidCoord().y);
			tmps.insert(pair<int,int>(dis, bd->getId()));
		}
	}


	multimap<int, int>::iterator it2;
	for(it2=tmps.begin(); it2!=tmps.end(); it2++)
	{
		if (candidate.size() >= 3)
		{
			break;
		} 
		else
		{
			candidate.push_back(it2->second);
		}
	}
}

void SoldierZsjqr::animationEvent( Armature *armature, MovementEventType movementType, const string& movementID )
{
	if (movementType == LOOP_COMPLETE || movementType == COMPLETE)
	{
		string temp = movementID;
		temp = temp.substr(0, temp.length()-2);
		char compstr[250];
		sprintf(compstr, "%02d_attack", 1);

		if (temp.compare(compstr) == 0)
		{
			bool isDead  = getTarget()->subHp(-SoldierConfig::getSoldierConfig(getType())->attack);
			if (isDead)
			{
				setStatus(e_battle_status_idle);
				m_direction = 0;
				getTarget()->dead();
				unschedule(schedule_selector(Soldier::updateAttack));
			}
			dead();
		}
	}
}


//火枪兵700005
SoldierHqb::SoldierHqb( int id ) : Soldier(id)
{

}

SoldierHqb::~SoldierHqb( void )
{

}

//void SoldierHqb::animationEvent( Armature *armature, MovementEventType movementType, const std::string& movementID )
//{
//	if (movementType == LOOP_COMPLETE || movementType == COMPLETE)
//	{
//		string temp = movementID;
//		temp = temp.substr(0, temp.length()-2);
//		char strAttack[15];
//		sprintf(strAttack, "%02d_attack", 1);
//		char strBoom[15];
//		sprintf(strBoom, "%02d_boom", 1);
//
//		auto target = getTarget();
//		if (temp.compare(strAttack) == 0)
//		{
//			auto parent = BattleScene::getPlaceLayer();
//			if (_fly == NULL)
//			{
//				auto sd = SoldierData::getSoldierData(getId());
//				int type = sd->type / 100;
//				char sType[10];
//				sprintf(sType,"%d",type);
//
//				_fly = Armature::create(sType);
//				_fly->setScale(0.5); 
//				parent->addChild(_fly);
//			} 
//			_fly->setVisible(true);
//
//			//TODO:修正弹道起始位置(还是有偏差)
//			Point p = Point(0, 0);
//			string temp2 = movementID;
//			temp2 = temp2.substr(temp2.length()-2, temp2.length());
//			if (temp2.compare("01") == 0)
//			{
//				p = Point(31, 13);
//			} 
//			else if (temp2.compare("02") == 0)
//			{
//				p = Point(52, 55);
//			}
//			else if (temp2.compare("03") == 0)
//			{
//				p = Point(42, 80);
//			}
//			else if (temp2.compare("04") == 0)
//			{
//				p = Point(0, 15);
//			}
//			else if (temp2.compare("05") == 0)
//			{
//				p = Point(17, 88);
//			}
//			Point p0 = MapHelper::convertCoordToPoint(getMidCoord());
//			_fly->setPosition(Point(p0.x + p.x, p0.y + p.y));
//
//			//修正弹道起始角度
//			float rotation = 0;
//			switch (getDirection())
//			{
//			case e_battle_dir_east :
//				rotation = 335;
//				break;
//			case e_battle_dir_es :
//				rotation = 0;
//				break;
//			case e_battle_dir_south :
//				rotation = 45;
//				break;
//			case e_battle_dir_ws :
//				rotation = 90;
//				break;
//			case e_battle_dir_west :
//				rotation = 135;
//				break;
//			case e_battle_dir_wn :
//				rotation = 180;
//				break;
//			case e_battle_dir_north :
//				rotation = 225;
//				break;
//			case e_battle_dir_en :
//				rotation = 270;
//				break;
//			default:
//				break;
//			}
//			_fly->setRotation(rotation);
//			_fly->getAnimation()->play("01_fly01");
//
//			//TODO:残影，设置方向
//			auto sc = SoldierConfig::getSoldierConfig(getType());
//			auto move = MoveTo::create(sc->fly_speed, MapHelper::convertCoordToPoint(target->getMidCoord()));
//			auto action = Sequence::create(move, CallFunc::create(CC_CALLBACK_0(SoldierPb::runBoomAction,this)), NULL);
//			_fly->runAction(action);
//
//			//flyMove(target->getMidCoord());
//		}
//		else if (temp.compare(strBoom) == 0)
//		{
//			if (_boom != NULL)
//			{
//				_boom->stopAllActions();
//				_boom->setVisible(false);
//			}
//			auto sc = SoldierConfig::getSoldierConfig(getType());
//			//if (target->subHp(-sc->attack))
//			//{
//			//	setStatus(e_soldier_tatus_idle);
//			//	unschedule(schedule_selector(Soldier::updateAttack));
//			//} 
//		}
//	}
//}

