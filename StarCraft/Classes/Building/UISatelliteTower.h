#ifndef  __UISATELLITETOWER_H
#define  __UISATELLITETOWER_H

#include "UILightingTower.h"

//TODO ������ �������α�
class UISatelliteTower : public UILightingTower
{
public:
	UISatelliteTower(){}
	~UISatelliteTower(){}
	CREATE_FUNC(UISatelliteTower);
	virtual bool initWithBattleLogic( DBuilding * db );
};

#endif



