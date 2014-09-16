#ifndef _EXITDLG_H_
#define _EXITDLG_H_

#include "UIPopupLayer.h"
#include "BuildingData.h"
#include "BuildingConfig.h"

#include "cocos2d.h"
using namespace cocos2d;
#include "extensions/cocos-ext.h"
using namespace cocos2d::extension;


class UIInfoDlg : public UIPopupLayer,public TableViewDataSource, public TableViewDelegate
{
public:
	UIInfoDlg();
	~UIInfoDlg();
    CREATE_FUNC(UIInfoDlg);
	static UIInfoDlg* CreateFromDB(DBuilding * db);
private:
	void createTimer();
	//创建进度条
	void createTimer(string str,int cur,int max,float offsetY = 0);
	//创建全体军队列表
	void createCamp();
	DBuilding * _db;
private:
	TableView * _tableview;
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell){}
	virtual Size tableCellSizeForIndex( TableView *table, ssize_t idx );
	virtual TableViewCell* tableCellAtIndex( TableView *table, ssize_t idx );
	virtual ssize_t numberOfCellsInTableView( TableView *table );

};

#endif