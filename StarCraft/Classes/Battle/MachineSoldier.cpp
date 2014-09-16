#include "MachineSoldier.h"
#include "SoldierData.h"
#include "SoldierConfig.h"



//机械战警700006
SoldierJxzj::SoldierJxzj( int id ) : Soldier(id)
{

}

SoldierJxzj::~SoldierJxzj( void )
{

}

//烈焰战车700007
SoldierLyzc::SoldierLyzc( int id ) : Soldier(id)
{
	auto data = SoldierData::getSoldierData(id);
	int type = data->type / 100;
	char sType[10];
	sprintf(sType,"%d",type);
	_head = nullptr;
	setHead(Armature::create(sType));
	
	_currentIndex = 0;
	_degreeIndex = 0;
	_head = nullptr;
}

SoldierLyzc::~SoldierLyzc( void )
{
	CC_SAFE_RELEASE(_head);
}

void SoldierLyzc::runWalkAction()
{
	string bodyName = "";
	string headName = "";
	switch (m_direction)
	{
	case e_battle_dir_east :
		bodyName = "01_walk03";
		headName = "01_head05";
		break;
	case e_battle_dir_es :
		bodyName = "01_walk02";
		headName = "01_head07";
		break;
	case e_battle_dir_south :
		bodyName = "01_walk01";
		headName = "01_head10";
		break;
	case e_battle_dir_ws :
		bodyName = "01_walk04";
		headName = "01_head13";
		break;
	case e_battle_dir_west :
		bodyName = "01_walk01";
		headName = "01_head10";
		break;
	case e_battle_dir_wn :
		bodyName = "01_walk02";
		headName = "01_head07";
		break;
	case e_battle_dir_north :
		bodyName = "01_walk03";
		headName = "01_head05";
		break;
	case e_battle_dir_en :
		bodyName = "01_walk05";
		headName = "01_head01";
		break;
	default:
		break;
	}

	getBody()->getAnimation()->play(bodyName);
	auto head = getHead();
	auto bone = getBody()->getBone("head");
	bone->addDisplay(head,0);
	bone->changeDisplayWithIndex(0,true);
	head->getAnimation()->play(headName);
}

void SoldierLyzc::doMove()
{
	if (getTarget()->getStatus() == E_STATUS_DEAD)
	{
		setStatus(e_battle_status_idle);
		m_direction = 0;
		return;
	}

	if (checkDistance())
	{
		//setStatus(e_battle_tatus_attack);
		double begin = getBeginDegree();
		double end = getEndDegree();
		_currentIndex = getIndexFormDegree(begin);
		_degreeIndex = getIndexFormDegree(end);
		rotate();
	} 
	else
	{
		Coord next = getNextCoord();
		if (next.x == -1 && next.y == -1)
		{
			//setStatus(e_battle_tatus_attack);
			double begin = getBeginDegree();
			double end = getEndDegree();
			_currentIndex = getIndexFormDegree(begin);
			_degreeIndex = getIndexFormDegree(end);
			rotate();
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

void SoldierLyzc::doAttack()
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
	}
	else
	{
		setStatus(e_battle_status_moveing);
		followTarget();
	}
}

double SoldierLyzc::getBeginDegree()
{
	double degree = 0;
	switch (getDirection())
	{
	case e_battle_dir_east:
		degree = 45;
		break;
	case e_battle_dir_es:
		degree = 90;
		break;
	case e_battle_dir_south:
		degree = 135;
		break;
	case e_battle_dir_ws:
		degree = 180;
		break;
	case e_battle_dir_west:
		degree = 225;
		break;
	case e_battle_dir_wn:
		degree = 270;
		break;
	case e_battle_dir_north:
		degree = 315;
		break;
	case e_battle_dir_en:
		degree = 0;
		break;
	default:
		break;
	}
	return degree;
	
}

double SoldierLyzc::getEndDegree()
{
	double degree = 0;
	//以m_coord为圆心，计算m_target->getMidCoord()的角度
	Coord coord1 = getTarget()->getMidCoord();
	Coord coord2 = getMidCoord();
	if (coord1.x == coord2.x && coord1.y == coord2.y)
	{
		degree = getBeginDegree();
	} 
	else
	{
		Point ptAnchor = Point(coord1.x, coord1.y);
		Point ptFirst = Point(coord2.x, coord2.y);
		Point ptSecond = Point(ptAnchor.x - 10, ptAnchor.y);
		float angle = MapHelper::getRotateAngle(ptAnchor, ptFirst, ptSecond);
		degree = angle * 180.0 / 3.1415926;
		if (coord2.y > coord1.y)
		{
			degree = 360 - degree;
		}
	}
	return degree;
}

void SoldierLyzc::rotate()
{
	//_degreeIndex = getIndexFormDegree(_attackDegree);

	getHead()->getAnimation()->play(getTowerStringFromLevel(1, _degreeIndex));
}

int SoldierLyzc::getIndexFormDegree(int degree)
{   
	//资源15度一个单位
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
	return tempindex;
}

string SoldierLyzc::getMovement( int from, int to )
{   
	int resutl = from;
	if (from != to)
	{
		int degreefrom = (from - 1) * 15;
		degreefrom = degreefrom <= 180 ? degreefrom : degreefrom - 360; 
		int degreeto   = (to - 1) * 15;
		degreeto = degreeto <= 180 ? degreeto : degreeto -360;
		int ss = degreeto - degreefrom;
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
	//return getTowerStringFromLevel(getLevel(),resutl);
	return getTowerStringFromLevel(1, resutl);
}

string SoldierLyzc::getTowerStringFromLevel( int level,int degree /*= 1*/ )
{
	auto sz =String::createWithFormat("%02d_head%02d",level,degree);
	return sz->getCString();
}

void SoldierLyzc::animationEvent( Armature *armature, MovementEventType movementType, const string& movementID )
{
	if (movementType == LOOP_COMPLETE || movementType == COMPLETE)
	{
		auto va = Value(movementID.substr(movementID.length() - 2,2).c_str());
		_currentIndex = va.asInt();
		if (_currentIndex != _degreeIndex)
		{
			getHead()->getAnimation()->play(getMovement(_currentIndex,_degreeIndex));
		}
		else
		{
			setStatus(e_battle_status_attack);
		}
	}
}

//攻城车700008
SoldierGcc::SoldierGcc( int id ) : Soldier(id)
{

}

SoldierGcc::~SoldierGcc( void )
{

}

void SoldierGcc::runWalkAction()
{
	string bodyName = "";
	switch (m_direction)
	{
	case e_battle_dir_east :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_es :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_south :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_ws :
		bodyName = "01_walk04";
		break;
	case e_battle_dir_west :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_wn :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_north :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_en :
		bodyName = "01_walk05";
		break;
	default:
		break;
	}

	getBody()->getAnimation()->play(bodyName);
}

//坦克700009
SoldierTk::SoldierTk( int id ) : Soldier(id)
{

}

SoldierTk::~SoldierTk( void )
{

}

void SoldierTk::runWalkAction()
{
	string bodyName = "";
	string headName = "";
	switch (m_direction)
	{
	case e_battle_dir_east :
		bodyName = "01_walk03";
		headName = "01_head05";
		break;
	case e_battle_dir_es :
		bodyName = "01_walk02";
		headName = "01_head07";
		break;
	case e_battle_dir_south :
		bodyName = "01_walk01";
		headName = "01_head10";
		break;
	case e_battle_dir_ws :
		bodyName = "01_walk04";
		headName = "01_head13";
		break;
	case e_battle_dir_west :
		bodyName = "01_walk01";
		headName = "01_head10";
		break;
	case e_battle_dir_wn :
		bodyName = "01_walk02";
		headName = "01_head07";
		break;
	case e_battle_dir_north :
		bodyName = "01_walk03";
		headName = "01_head05";
		break;
	case e_battle_dir_en :
		bodyName = "01_walk05";
		headName = "01_head01";
		break;
	default:
		break;
	}

	getBody()->getAnimation()->play(bodyName);
	auto head = getHead();
	auto bone = getBody()->getBone("head");
	bone->addDisplay(head,0);
	bone->changeDisplayWithIndex(0,true);
	head->getAnimation()->play(headName);
}

//雷神机器人700010
SoldierLsjqr::SoldierLsjqr( int id ) : Soldier(id)
{

}

SoldierLsjqr::~SoldierLsjqr( void )
{

}
