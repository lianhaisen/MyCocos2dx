#ifndef __SOLDIERFACTORY_H__
#define __SOLDIERFACTORY_H__

#include "Soldier.h"

class SoldierFactory
{
public:
	//创建士兵，id为兵种id，如70000101
	static Soldier* createSoldier(int id);

 
};

#endif // __SOLDIERFACTORY_H__
