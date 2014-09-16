#ifndef  __UIUPGRADEDIALOG_H_
#define  __UIUPGRADEDIALOG_H_

#include "UIPopupLayer.h"
class DBuilding;
using namespace std;

class UIUpgradeDialog: public UIPopupLayer
{
public:
	UIUpgradeDialog();
	~UIUpgradeDialog();
	CREATE_FUNC(UIUpgradeDialog);
	static UIUpgradeDialog* CreateFromDB(DBuilding * db);
private:
    void createTimer();
	void createTimer(string str,int cur,int next,int max,float offsetY = 0);
	void onUpgrade(Ref* ref);
	//延时关闭,否则直接崩溃
	void onClose();
private:
	DBuilding * _db;
};






#endif



