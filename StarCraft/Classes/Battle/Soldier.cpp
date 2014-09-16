#include "Soldier.h"
#include "AppMacros.h"
#include "AStar.h"
#include "SoldierConfig.h"
#include "SoldierData.h"
#include "BuildingConfig.h"
#include "BuildingData.h"
#include "BuildingObserver.h"
#include "BattleData.h"


Soldier::Soldier(int id)
{
	auto data = SoldierData::getSoldierData(id);

	setId(id);
	setType(data->type);
	setStatus(e_battle_status_idle);
	auto config = SoldierConfig::getSoldierConfig(data->type);
	setHp(config->max_hp);

	m_pathIndex = 0;
	m_direction = 0;

	_body = nullptr;
	_hpProcess = nullptr;
	_hpBg = nullptr;
	_newCoord = nullptr;
}

Soldier::~Soldier(void)
{
	m_path.clear();
	
	//modify by lhs  不能这样用 要先删除容器里面的对象然后在释放,否则容器会访问野指针导致崩溃
	//CC_SAFE_DELETE(_newCoord);
    
	//析构函数里面用
	CC_SAFE_RELEASE(_hpProcess);
	CC_SAFE_RELEASE(_hpBg);
	CC_SAFE_RELEASE(_body);

	_flys.clear();
	_booms.clear();
}

void Soldier::createBody()
{
	setBody(Armature::create(getArmatureName()));
	auto size = getBody()->getContentSize();
	setContentSize(size);
	addChild(getBody());
	getBody()->setPosition(size.width/2, 0);
	
	setScale(0.5);
	ignoreAnchorPointForPosition(false);
	setAnchorPoint(Point(0.5, 0));
}

void Soldier::createHpProgress()
{
	setHPBg(Sprite::create("ui/hp2.png"));
	addChild(getHPBg());
	getHPBg()->setPosition(Point(getContentSize().width / 2 ,getContentSize().height + 20));
	setHPProgress(ProgressTimer::create(Sprite::create("ui/hp.png")));
	addChild(getHPProgress());
	getHPProgress()->setType(ProgressTimer::Type::BAR);
	getHPProgress()->setMidpoint(Point(0,100));
	getHPProgress()->setPosition(Point(getContentSize().width / 2 ,getContentSize().height + 20));
	getHPProgress()->setBarChangeRate(Point(1,0));
	getHPProgress()->setPercentage(100);
}

void Soldier::updateHpProgress()
{
	if (getHPProgress() == nullptr)
	{
		createHpProgress();
	}
	auto config = SoldierConfig::getSoldierConfig(getType());
	float per = (float)getHp()/config->max_hp*100;
	getHPProgress()->setPercentage(per);
}

void Soldier::showUI()
{
	if (getBody() == nullptr)
	{
		createBody();
	}
}

void Soldier::startLogic()
{
	Director::getInstance()->getScheduler()->schedule(schedule_selector(Soldier::updateLogic), this, SCHEDULE_INTERVAL, false);
}

void Soldier::updateLogic( float dt )
{
	switch (getStatus())
	{
	case e_battle_status_idle:
		log("case e_battle_tatus_idle");
		setStatus(e_battle_status_path);
		break;
	case e_battle_status_relax:
		break;
	case e_battle_status_path:
		{
			log("case e_battle_tatus_path");
			setStatus(e_battle_status_pathing);
			vector<BattleLogic*> candidate;
			findCandidateTarget(candidate);
			if (candidate.empty())
			{
				setStatus(e_battle_status_idle);
			} 
			else
			{
				auto target = findTargetFromCandidate(candidate);
				setTarget(target);
				setStatus(e_battle_status_move);
			}
			break;
		}
	case e_battle_status_pathing:
		break;
	case e_battle_status_move:
		{
			log("case e_battle_tatus_move");
			setStatus(e_battle_status_moveing);
			doMove();
			break;
		}
	case e_battle_status_moveing:
		break;
	case e_battle_status_attack:
		{
			setStatus(e_battle_status_attacking);
			doAttack();
			break;
		}
	case e_battle_status_attacking:
		break;
	case e_battle_status_dead:
		release();
		break;
	case e_battle_status_deaded:
		break;
	default:
		setStatus(e_battle_status_idle);
		break;
	}
}

void Soldier::updateAttack(float dt)
{
	if (getTarget()->getStatus() == E_STATUS_DEAD)
	{
		setStatus(e_battle_status_idle);
		m_direction = 0;
		unschedule(schedule_selector(Soldier::updateAttack));
	}
	else
	{
		getBody()->getAnimation()->play(m_attackName, -1, 0);
	}
}

void Soldier::setCoord( Coord coord ) 
{  
	BattleLogic::setCoord(coord);
	if (_newCoord == nullptr)
	{
		_newCoord = BattleData::getInstance()->getSoldierCoord(coord);
		_newCoord->soldierlist.push_back(this);
	}
	else
	{   
		for(auto i = _newCoord->soldierlist.begin(); i != _newCoord->soldierlist.end(); i++)
		{
			if (*i == this)
			{   
				_newCoord->soldierlist.erase(i);
				break;
			}
		}
		_newCoord = BattleData::getInstance()->getSoldierCoord(coord);
		_newCoord->soldierlist.push_back(this);
	}
}

void Soldier::findCandidateTarget(vector<BattleLogic*> &candidate)
{
	auto soldier = SoldierConfig::getSoldierConfig(getType());
	multimap<int, BattleLogic*> tmps;
	if (soldier->attack_target == e_attack_target_none)
	{
		return;
	}
	else if (soldier->attack_target == e_attack_target_ground 
		|| soldier->attack_target == e_attack_target_ground2
		|| soldier->attack_target == e_attack_target_all)
	{
		//从建筑物中筛选目标
		//TODO:敌方士兵和建筑也是待选攻击目标（从_attacker或_defender中筛选）
		auto bds = BattleData::getInstance()->getDefendBuilding();
		//优先处理攻击偏好为塔防和资源的士兵
		if (soldier->preference == e_preference_tower || soldier->preference == e_preference_resource)
		{
			map<int,BattleLogic*>::iterator it;
			for(it=bds.begin(); it!=bds.end(); it++)
			{
				auto building = it->second;
				if (building->getStatus() == E_STATUS_DEAD)
				{
					continue;
				}
				auto config = BuildingConfig::getBuildingConfig(building->getType());
				if (soldier->preference == config->building_type)
				{
					int dis = (building->getMidCoord().x - getMidCoord().x)*(building->getMidCoord().x - getMidCoord().x) + (building->getMidCoord().y - getMidCoord().y)*(building->getMidCoord().y - getMidCoord().y);
					tmps.insert(pair<int, BattleLogic*>(dis, building));
				}
			}
		}
		//无攻击偏好
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
				if (config->building_type == e_preference_facility)
				{
					//TODO:设施类特殊处理
					continue;
				}

				int dis = (bd->getMidCoord().x - getMidCoord().x)*(bd->getMidCoord().x - getMidCoord().x) + (bd->getMidCoord().y - getMidCoord().y)*(bd->getMidCoord().y - getMidCoord().y);
				tmps.insert(pair<int, BattleLogic*>(dis, bd));
			}
		}
	}

	multimap<int, BattleLogic*>::iterator it2;
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

BattleLogic* Soldier::findTargetFromCandidate(vector<BattleLogic*> candidate)
{
	log("findAttackTarget");
	BattleLogic* target = nullptr;
	unsigned int step = 9999;
	vector<Coord> tmpPath;


	MapHelper *map = new MapHelper();
	//auto bds = BattleData::getInstance()->getDefendBuilding();
	for (auto item : candidate)
	{
		//auto bd = bds.at(item);
		AStar *a = new AStar();
		a->setMap(map);
		a->setStartPoint(item->getMidCoord());
		a->setEndPoint(getMidCoord());
		a->startSearch();
		vector<Coord> path;
		a->getPath(path);

		if (path.size() < step)
		{
			target = item;
			step = path.size();
			tmpPath = path;
		}
	}

	setPath(tmpPath);

	return target;
}

void Soldier::doMove()
{
	if (getTarget()->getStatus() == E_STATUS_DEAD)
	{
		setStatus(e_battle_status_idle);
		m_direction = 0;
		return;
	}

	if (checkDistance())
	{
		setStatus(e_battle_status_attack);
		setAttackDirection();
	} 
	else
	{
		Coord next = getNextCoord();
		if (next.x == -1 && next.y == -1)
		{
			setStatus(e_battle_status_attack);
			setAttackDirection();
		} 
		else
		{
			//checkBeAttack();
			if (setMoveDirection(next))
			{
				runWalkAction();
			}
			auto sc = SoldierConfig::getSoldierConfig(getType());
			auto move = MoveTo::create(1/sc->move_speed, MapHelper::convertCoordToPoint(next));
			runAction(Sequence::create(move, CallFunc::create(CC_CALLBACK_0(Soldier::doMove,this)), NULL));
			setLocalZOrder(next.y);
			setCoord(next);
		}
	}
}

void Soldier::doAttack()
{
	//TODO:如果攻击目标是可移动的（比如敌方士兵，英雄），则每次攻击前都要判断是否在攻击范围内，如果不在范围内则重新寻路，或者更换目标
	if (checkDistance())
	{
		m_attackName = "";
		switch (m_direction)
		{
		case e_battle_dir_east :
			m_attackName = "01_attack03";
			break;
		case e_battle_dir_es :
			m_attackName = "01_attack02";
			break;
		case e_battle_dir_south :
			m_attackName = "01_attack01";
			break;
		case e_battle_dir_ws :
			m_attackName = "01_attack04";
			break;
		case e_battle_dir_west :
			m_attackName = "01_attack01";
			break;
		case e_battle_dir_wn :
			m_attackName = "01_attack02";
			break;
		case e_battle_dir_north :
			m_attackName = "01_attack03";
			break;
		case e_battle_dir_en :
			m_attackName = "01_attack05";
			break;
		default:
			break;
		}

		getBody()->getAnimation()->play(m_attackName, -1, 0);
		auto sc = SoldierConfig::getSoldierConfig(getType());
		Director::getInstance()->getScheduler()->schedule(schedule_selector(Soldier::updateAttack), this, sc->attack_speed, false);
		getBody()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Soldier::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		getBody()->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Soldier::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	}
	else
	{
		setStatus(e_battle_status_moveing);
		followTarget();
	}
}

bool Soldier::subHp( int hp )
{
	updateHpProgress();
	return BattleLogic::subHp(hp);
}

void Soldier::dead()
{   
	setStatus(e_battle_status_dead);
}

void Soldier::release()
{
	if (_newCoord)
	{   
		for(auto i = _newCoord->soldierlist.begin(); i != _newCoord->soldierlist.end(); i++)
		{
			if (*i == this)
			{   
				_newCoord->soldierlist.erase(i);
				break;
			}
		}
		_newCoord = nullptr;
	}

	if (getHPProgress())
	{
		getHPProgress()->removeFromParent();
	}
	if (getHPBg())
	{
		getHPBg()->removeFromParent();
	}
	if (getBody())
	{
		getBody()->removeFromParent();
	}

	setHPProgress(nullptr);
	setHPBg(nullptr);
	setBody(nullptr);

	unschedule(schedule_selector(Soldier::updateAttack));
	unschedule(schedule_selector(Soldier::updateLogic));
	removeFromParentAndCleanup(true);
}

void Soldier::setPath(vector<Coord> path)
{
	m_path.clear();
	m_path = path;
	m_pathIndex = 0;
}

bool Soldier::setMoveDirection( Coord next )
{
	int direction = 0;
	if (next.x < getMidCoord().x)
	{
		if (next.y > getMidCoord().y)
		{
			direction = e_battle_dir_es;
		} 
		else if (next.y < getMidCoord().y)
		{
			direction = e_battle_dir_en;
		}
		else
		{
			direction = e_battle_dir_east;
		}
	}
	if (next.x == getMidCoord().x)
	{
		if (next.y > getMidCoord().y)
		{
			direction = e_battle_dir_south;
		} 
		else
		{   
			getBody()->setScaleX(-1);
			direction = e_battle_dir_north;
		}
	}
	if (next.x > getMidCoord().x)
	{
		getBody()->setScaleX(-1);
		if (next.y > getMidCoord().y)
		{
			direction = e_battle_dir_ws;
		} 
		else if (next.y < getMidCoord().y)
		{
			direction = e_battle_dir_wn;
		}
		else
		{
			direction = e_battle_dir_west;
		}
	}

	if (direction == m_direction)
	{
		m_direction = direction;
		return false;
	} 
	else
	{
		m_direction = direction;
		return true;
	}
}

void Soldier::setAttackDirection()
{
	int direction = 0;
	Coord coord1 = getMidCoord();
	Coord coord2 = getTarget()->getMidCoord();
	Point point1 = MapHelper::convertCoordToPoint(coord1);
	Point point2 = MapHelper::convertCoordToPoint(coord2);
	auto vr = (point2 - point1).getNormalized();
	auto degree = int(atan2(vr.x,vr.y) * 180 / M_PI);

	if (degree < 0)
	{
		degree += 360;
	}
	if (degree >= 360)
	{
		degree %= 360;
	}

	getBody()->setScaleX(1);
	if (degree <= 22 )
	{
		direction = e_battle_dir_en;
	} 
	else if (degree > 22 && degree < 73)
	{
		direction = e_battle_dir_east;
	}
	else if (degree >= 73 && degree <= 112)
	{
		direction = e_battle_dir_es;
	}
	else if (degree > 112 && degree < 163)
	{
		direction = e_battle_dir_south;
	}
	else if (degree >= 163 && degree <= 202)
	{
		direction = e_battle_dir_ws;
	}
	else if (degree > 202 && degree < 253)
	{
		getBody()->setScaleX(-1);
		direction = e_battle_dir_west;
	}
	else if (degree >= 253 && degree <= 292)
	{
		getBody()->setScaleX(-1);
		direction = e_battle_dir_wn;
	}
	else if (degree > 292 && degree < 343)
	{
		getBody()->setScaleX(-1);
		direction = e_battle_dir_north;
	}
	else if (degree >= 343)
	{
		direction = e_battle_dir_en;
	}

	if (direction > 0)
	{
		m_direction = direction;
	}
}

void Soldier::runWalkAction()
{
	string name = "";
	switch (m_direction)
	{
	case e_battle_dir_east :
		name = "01_walk03";
		break;
	case e_battle_dir_es :
		name = "01_walk02";
		break;
	case e_battle_dir_south :
		name = "01_walk01";
		break;
	case e_battle_dir_ws :
		name = "01_walk01";
		break;
	case e_battle_dir_west :
		name = "01_walk01";
		break;
	case e_battle_dir_wn :
		name = "01_walk02";
		break;
	case e_battle_dir_north :
		name = "01_walk03";
		break;
	case e_battle_dir_en :
		name = "01_walk03";
		break;
	default:
		break;
	}

	showUI();
	getBody()->getAnimation()->play(name);
}

int Soldier::getDirection()
{
	return m_direction;
}

void Soldier::followTarget()
{
	if (getTarget()->getStatus() == E_STATUS_DEAD)
	{
		setStatus(e_battle_status_idle);
		m_direction = 0;
		return;
	}
	MapHelper *map = new MapHelper();
	AStar *a = new AStar();
	a->setMap(map);
	a->setStartPoint(getTarget()->getMidCoord());
	a->setEndPoint(getMidCoord());
	a->startSearch();
	vector<Coord> path;
	a->getPath(path);
	setPath(path);

	m_direction = 0;
	doMove();
}

Coord Soldier::getNextCoord()
{
	if (m_path.size() > 0 && m_pathIndex < m_path.size() - 1)
	{
		m_pathIndex++;
		return m_path.at(m_pathIndex);
	} 
	else
	{
		//返回非法坐标
		return	Coord(-1, -1);
	}
}

bool Soldier::checkDistance()
{
	auto sc = SoldierConfig::getSoldierConfig(getType());
	//TODO:扣除受击范围
	int powX = pow(abs((int)getMidCoord().x - (int)getTarget()->getMidCoord().x), 2);
	int powY =  pow(abs((int)getMidCoord().y - (int)getTarget()->getMidCoord().y), 2);
	int distance = powX + powY;
	if (distance <= pow(sc->max_range + 2, 2))
	{ 
		return true;
	} 
	else
	{
		return false;
	}
}

void Soldier::animationEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
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
		}
	}
}

void Soldier::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{  
	if(strcmp(evt.c_str(),"attack") == 0)  
	{  
		log("Soldier::onFrameEvent attack");
	}  
}  

NewCoord* Soldier::getNewCoord()
{
	return _newCoord;
}

Armature* Soldier::getBody()
{
	if (_body == nullptr)
	{
		createBody();
	}
	return _body;
}

void Soldier::setBody( Armature* body )
{   
	if (_body != nullptr)
	{
		_body->release();
	}
	_body = body;
	if (_body != nullptr)
	{
		_body->retain();
	}
    
}

std::string Soldier::getArmatureName()
{
	char sType[10];
	sprintf(sType,"%d", getType()/100);

	return sType;
}

Armature * Soldier::getIdleFly()
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
		result = Armature::create(getArmatureName());
		result->setScale(getScale());
		_flys.pushBack(result);
	}
	return result;
}

Armature * Soldier::getIdleBoom()
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
		result = Armature::create(getArmatureName());
		result->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Soldier::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		result->setScale(getScale());
		_booms.pushBack(result);
	}
	return result;
}
