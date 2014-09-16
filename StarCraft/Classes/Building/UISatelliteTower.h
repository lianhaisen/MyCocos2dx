#ifndef  __UISATELLITETOWER_H
#define  __UISATELLITETOWER_H

#include "UILightingTower.h"

//TODO ÎÀÐÇËþ ¼ì²é³öÒþÐÎ±ø
class UISatelliteTower : public UILightingTower
{
public:
	UISatelliteTower(){}
	~UISatelliteTower(){}
	CREATE_FUNC(UISatelliteTower);
	virtual bool initWithBattleLogic( DBuilding * db );
};

#endif



