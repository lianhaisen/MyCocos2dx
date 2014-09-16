#ifndef __UIConveyorLayer_H__
#define __UIConveyorLayer_H__
/*
	运输机
*/
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "UITouchButton.h"
#include "UISoldierHeadLayer.h"
#include "UITipLayer.h"
using namespace cocos2d;
using namespace ui;
#define CONVEYORID 700001 //运输机id
class UIConveyorLayer :public UITipLayer
{
public:

public:
	UIConveyorLayer();
	~UIConveyorLayer();
    CREATE_FUNC(UIConveyorLayer);
	virtual bool init();
	//static UIBarracks * CreateFromDB(DBuilding * db);
private:
	void initWithForm();

	Node* getOneItem(Node* Itme1,Node* Item2);
	//运输机列表
	void createConveyorList();

	//运输机内信息
	void showConveyorInfoLayer();
	void UpDataConveyorInfo(int id,int TypeId);

	//士兵信息列表
	void showSoldierInfoLayer();
	void UpDataSoldierInfo(int id,int TypeId);

	//添加士兵到运输机
	void addSoldier(Ref *sender);
	//减少运输机士兵
	void ReductionSoldier(Ref *sender);
private:
	int m_iCurState;
	ListView* m_ConveyorInfolist;
	ListView* m_SoldierInfolist;


};




#endif //  __UIHeroPub_H__


