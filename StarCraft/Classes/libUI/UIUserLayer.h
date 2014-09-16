#ifndef __UIUSERLAYER_H__
#define __UIUSERLAYER_H__

#include "cocos2d.h"
using namespace cocos2d;
#include "UIPanZoomLayer.h"
#include "UIUserInfoLayer.h"
#include "UITouchButton.h"
class UIBuildingPopLayer;
class DBuilding;
class BuyListConfig;
class UIUserLayer :public Scene,public UILongPressOrClickDelegate 
{
public:
    UIUserLayer(void);
    ~UIUserLayer(void);
    virtual bool init();  
    CREATE_FUNC(UIUserLayer);

	//测试摆放
	void TestPlaceBuilding();

	//建筑物逻辑
	void TestLogic2();
private:
	// 长按
	virtual void doLongPress(PointArray* array);
	// 点击
	virtual void doClick(PointArray* array);
	// 检查是否能点击或者长按
	virtual bool checkIsCanClickOrLongPress(PointArray* array);

	void buildingSelect(Ref* sender);
    void buttonClick(Ref * ref);
	void dlgClose(Ref * ref);

	void onBuy(Ref * ref);
	void onBuyReturn(Ref * ref,BuyListConfig * cfg);

private:
	UIPanZoomLayer *     _zoomlayer;
	UIBuildingPopLayer * _popLayer; 
	DBuilding * _selectDB;
	Vector<UIUserInfoLayer*> m_vTopUi;
	Vector<UIUserInfoLayer*> m_vRightUi;
	Vector<UITouchButton*> m_vWarBtnUi;
	bool _bPopUp;
};

#endif // __MAINSCENE_H__