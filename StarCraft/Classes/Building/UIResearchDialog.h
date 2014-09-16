#ifndef _UIRESEARCHDIALOG_H  
#define _UIRESEARCHDIALOG_H

#include "UIPopupLayer.h"
#include "BuildingData.h"

class UITipLayer;
class UISoldierItem;

class UIResearchDialog : public UIPopupLayer
{
public:
	UIResearchDialog();
	~UIResearchDialog(){}
	CREATE_FUNC(UIResearchDialog);
	static UIResearchDialog * CreateFromDB(DBuilding * db);
private:
	 void initUpgradeSoldier(bool isfirst = true);
	 void initSoldierItems();
	 void updateTimer(float dt);
	 void onSolderItemClick(Ref* ref);
	 void onUpgrade(Ref * ref);

	 DBuilding * _db;
	 UITipLayer * _tiplayer;
	 UISoldierItem * _selectItem;
	 

};




#endif



