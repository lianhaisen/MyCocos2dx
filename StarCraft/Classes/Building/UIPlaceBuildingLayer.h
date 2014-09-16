#ifndef __UIPLACEBUILDINGLAYER_H__
#define __UIPLACEBUILDINGLAYER_H__

#include "cocos2d.h"
using namespace cocos2d;
#include "UIBuildingBase.h"
#include "UIWall.h"

//TODO购买模式

class UIPlaceBuildingLayer : public Node
{

public:
	enum class BLayerStatus
	{  
		//平常
		NOMAL = 0,
		//购买状态
		BUY   = 1
	};
	UIPlaceBuildingLayer();
	~UIPlaceBuildingLayer();
	virtual bool init();
	CREATE_FUNC(UIPlaceBuildingLayer);
  
    
	void AddBuilding(Building * building,int zorder = 0);
	void AddNewBuilding(Building * building);

	bool getIsMove(){return _isMoving;}
	void setIsInBattle(bool isInbattle);

	void addbuildPoint(int x,int y,int gridcount);//添加建筑物点
	void removebuildingPoint(int x,int y,int gridcount);//移动时去掉当前的点
	bool checkaddPoint(int x,int y,int gridcount);//测试是否可以放置

	WallCoord * getWallCoord(Coord coord);
	UIWall    * getWallFromCoord(Coord coord);
 private:

	bool onTouchesBegan(Touch * touch, cocos2d::Event  *event);
	void onTouchesMoved(Touch * touch, cocos2d::Event  *event);
	void onTouchesEnded(Touch * touch, cocos2d::Event  *event);
	void onTouchCancelled(Touch * touch, cocos2d::Event  *event);

    virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
	void onDraw(const kmMat4 &transform, bool transformUpdated);
    CustomCommand _customCommand;
    Building* getItemForTouch(Touch * touch);
	bool  isInTouch(Building * item,Touch * touch);

private:
	void addWalls(Building * building);
	std::map<int,WallCoord*> _walls;



    std::vector<Building *> _buildings;
 

	EventListenerTouchOneByOne * _lister;
	Building * _selectItem;
	Building * _chooseItem;
	Point		 _touchBeginPoint;
	Point        _touchDiff;
	bool         _isMoving;
	bool         _isMoveScreen;
private: //TOOD
	std::set<int> _pointSet; //建筑物数据


	//购买的建筑
	Building * _buyBuliding;
	//层状态(购买 or 正常)
	BLayerStatus _LayerStatus;
	void onBuyYesOrNo(Ref* ref,bool isyes);


};






#endif