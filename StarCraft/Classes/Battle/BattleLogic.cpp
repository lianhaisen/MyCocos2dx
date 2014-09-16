#include "BattleLogic.h"
#include "BattleData.h"


BattleLogic::BattleLogic( void )
{
	_target = nullptr;
}

BattleLogic::~BattleLogic( void )
{

}

void BattleLogic::setId(int id)
{
	_id = id;
}

int BattleLogic::getId()
{
	return _id;
}

int BattleLogic::getType()
{
	return _typeid;
}

void BattleLogic::setType( int type )
{
	_typeid = type;
}

E_BattleStatus BattleLogic::getStatus()
{
	return _status;
}

void BattleLogic::setStatus( E_BattleStatus status )
{
	_status = status;
}

Coord BattleLogic::getCoord()
{
	return _coord;
}

void BattleLogic::setCoord( Coord coord )
{
	_coord = coord;
}

int BattleLogic::getHp()
{
	return _hp;
}

void BattleLogic::setHp( int hp )
{
	_hp = hp;
	_maxHp = hp;
}

bool BattleLogic::subHp( int hp )
{
	if (hp == 0)
	{
		return false;
	}

	_hp += hp;
	if (_hp <= 0)
	{
		_hp = 0;
		return true;
	}
	else if (_hp >= _maxHp)
	{
		_hp = _maxHp;
		return true;
	}
	else
	{
		return false;
	}
}

bool BattleLogic::isFullHp()
{
	if (_hp >= _maxHp)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BattleLogic* BattleLogic::getTarget()
{
	return _target;
}

void BattleLogic::setTarget( BattleLogic* target )
{
	_target = target;
}





