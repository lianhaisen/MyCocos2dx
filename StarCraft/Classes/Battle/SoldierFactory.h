#ifndef __SOLDIERFACTORY_H__
#define __SOLDIERFACTORY_H__

#include "Soldier.h"

class SoldierFactory
{
public:
	//����ʿ����idΪ����id����70000101
	static Soldier* createSoldier(int id);

 
};

#endif // __SOLDIERFACTORY_H__
