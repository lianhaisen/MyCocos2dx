
/*
用法:在调用类继承　　UILongPressOrClickDelegate
重载需要的方法

然后　SetDelegate　就可使用

TODO: 触摸可做优化,少量位移可以不当作移动
TOOD: _getPointArray 坐标转换
*/



// Created By GrowingDever 21th January 2014

#ifndef _PAN_ZOOM_LAYER_H_
#define _PAN_ZOOM_LAYER_H_

#include "cocos2d.h"
class UIPlaceBuildingLayer;
class Building;
using namespace cocos2d;



// 长按或者点击的委托
class UILongPressOrClickDelegate
{
public:
    // 长按
    virtual void doLongPress(PointArray* array){CC_UNUSED_PARAM(array);}
    // 点击
    virtual void doClick(PointArray* array){CC_UNUSED_PARAM(array);}
    // 检查是否能长按
    virtual bool checkIsCanClickOrLongPress(PointArray* array){CC_UNUSED_PARAM(array);return false;}
};



class UIPanZoomLayer : public LayerColor
{
private:
	Vector<Touch*> _touches;
	Point _beganTouchPoint;
	Point _endedTouchPoint;
	Point _deltaSum;
	Point _prevDeltaPoint;
	double _accelerationFactor;
	struct timeval _timeStamp;
	Rect _panBoundsRect;
	float _maxScale;
	float _minScale;
	float _productFactor;

	bool _isHolding;

	bool m_bIsNeedLongTouch;
	bool m_bIsLongTouched;
	bool m_bIsMaybeLongTouched;
	float m_lTouchBeganTime;
    UILongPressOrClickDelegate * _delegate;
    float _getMillisecondTime();
    
    //获得在图中实际的接触的点   
 	PointArray * _getPointArray();
	
	Node * _map;
    UIPlaceBuildingLayer * _buildinglayer;

public:
	UIPanZoomLayer();
	virtual ~UIPanZoomLayer();

	static UIPanZoomLayer* create();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual void update( float dt );

	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event  *event);


	virtual void setPosition( Point position );
	virtual void setScale( float scale );

	void SetPanBoundsRect( Rect rect );
	void SetMaxScale( float maxScale );
	float GetMaxScale();
	void SetMinScale( float minScale );
	float GetMinScale();

	void Holding();
	void UnHolding();

	void SetProductFactor( float v );
    
    //设置委托
    void SetDelegate(UILongPressOrClickDelegate * delegate);
    
	
    //************************************
    // Method:    setMap 设置地图
    // FullName:  UIPanZoomLayer::setMap
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: Sprite * map
    //************************************
    void setMap(Node * map,bool isInbattle = false);

	//添加建筑
	void addBuilding(Building * build,int zorder = 0);
	void addNewBuilding(Building * build);


	//获得摆放层
	UIPlaceBuildingLayer * getPlaceLayer();
};


#endif