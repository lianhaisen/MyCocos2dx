#ifndef __TESTSCENE_H__
#define __TESTSCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
#include "UIPanZoomLayer.h"
class UIBuildingPopLayer;
class DBuilding;
class BuyListConfig;
#include "UIUserMinUi.h"


class TestScene :public Scene,public UILongPressOrClickDelegate 

{
public:
    TestScene(void);
    ~TestScene(void);
    virtual bool init();  
    CREATE_FUNC(TestScene);

	//测试摆放
	void TestPlaceBuilding();

	//建筑物逻辑
	void TestLogic2();

	//
	void TestMainMap();

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

    void onWarClick(Ref * ref);

private:
	UIPanZoomLayer *     _zoomlayer;
	UIBuildingPopLayer * _popLayer; 
	DBuilding * _selectDB;
	UIUserMinUi*         _miniUI;
};

#endif // __MAINSCENE_H__
