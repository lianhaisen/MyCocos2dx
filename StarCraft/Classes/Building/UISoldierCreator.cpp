#include "UISoldierCreator.h"
#include "BarrackData.h"
#include "BuildingData.h"
#include "BuildingFunc.h"

void UISoldierCreator::setBuidingData( DBuilding * db )
{
	Building::setBuidingData(db);
	this->schedule(schedule_selector(UISoldierCreator::updateWorkStatus),1.0f);
}

void UISoldierCreator::updateWorkStatus( float dt )
{    

    if (getBuidingData() )
    {
		auto item = TrainingData::getTrainingDataFromId(getBuidingData()->id);
		if (item)
		{
			if (item->soldieritems.size() > 0)
			{
				auto first = item->soldieritems.front();
				if (item->curtraininglavetime > 1)
				{
					item->curtraininglavetime--;
				}
				else if (item->curtraininglavetime == 0)
				{
					item->curtraininglavetime = first->trainingtime - 1;
				}
				else
				{
					first->number --;
				    BuildingFunc::getInstance()->addSoldier(getBuidingData()->coord,first->soldierid,first->population);
                     
					if (first->number <= 0)
					{
						item->soldieritems.pop_front();
						CC_SAFE_DELETE(first);
						if (item->soldieritems.size() > 0)
						{
							first = item->soldieritems.front();
							item->curtraininglavetime = first->trainingtime;
						}
						else
						{
							item->curtraininglavetime = 0;
						}
					}
					else
					{
						item->curtraininglavetime = first->trainingtime;
					}

					item->updateData();
				}
			}
		}
    }
}


