#include "UISatelliteTower.h"




bool UISatelliteTower::initWithBattleLogic( DBuilding * db )
{
	auto item = Building::initWithBattleLogic(db);
	setTarget(nullptr);
	initWatchlist();
	return item;
}
