#ifndef __UIBUYBUILDINGLAYER_H
#define __UIBUYBUILDINGLAYER_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class BuyListConfig;
typedef std::function<void(Ref*,BuyListConfig*)> ccBtnCallBack;

class UIBuyBuildingLayer : public LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
	UIBuyBuildingLayer();
	~UIBuyBuildingLayer();
	virtual bool init();
	CREATE_FUNC(UIBuyBuildingLayer);

	bool onTouchesBegan(Touch * touch, cocos2d::Event  *event);
	void onTouchesMoved(Touch * touch, cocos2d::Event  *event);
	void onTouchesEnded(Touch * touch, cocos2d::Event  *event);
	void onTouchCancelled(Touch * touch, cocos2d::Event  *event);
     
	virtual void scrollViewDidScroll(ScrollView* view) {}
	virtual void scrollViewDidZoom(ScrollView* view) {}
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	void setTarget(const ccBtnCallBack& btncallback){_btnCallback = btncallback;}

private:
	CC_SYNTHESIZE_RETAIN(Sprite*,_bg,Bg);
	void initBg();

	Vector<Sprite*> _taps;
	Sprite * _selectSp;
	Sprite * _curSp;

	Sprite * getItemForTouch(Touch * touch);
	bool  isTouchEdge(Touch * touch);
	void onCardClick(Ref* sender);
	
	TableView * _tableview;
	int         _curtag;
    ccBtnCallBack _btnCallback;

	
};



#endif



