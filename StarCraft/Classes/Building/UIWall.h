#ifndef _UIWALL_H
#define _UIWALL_H

#include "UIBuildingBase.h"

//TODO 检查BUG

class UIWall;
class WallCoord
{   
public:
	WallCoord(){}
    ~WallCoord();
    int key;//x*MAX_GRID_COUNT+y
	//防止出现重叠的情况
	Vector<UIWall*> walllist;
};

class UIWall : public Building
{
public:
	enum class WallStatus
	{   
		//正常
		NOMAL = 1,
		//右边
		RIGHT = 2,
		//左边
		LEFT = 3,
		//中间
		MIDDLE = 4
	};

	enum  class RowSelectStatus
	{  
		//正常
		NOMAL = 1,
		//行选择
		ROWSELECTE = 2,
	};

	 UIWall();
	~UIWall();
	 CREATE_FUNC(UIWall);
	 void  releaseWallCoord();
	 void  setWallCoord(Coord coor);
	 RowSelectStatus getRowStatus(){return _rowStatus;}
	 void setRowSelectStatus(RowSelectStatus status){_rowStatus = status;} 
	 //选择当前行
	 void getCurrntRow();
	 //旋转行
	 void RotateRow();
private: 
	void  CheckWallStatus(Coord coor);
	void  CheckOtherWall(Coord coor);
	virtual void updateStatus();
	virtual void setMove( bool isMove );
	virtual void setCoor( Coord coor );
	virtual void unselected();
	//改变箭头
	void changeArr();
	//还原箭头
	void resertArr();
	virtual bool isPointInRect( const Point& point );
	virtual void addBuildPoint();
	virtual void removeBuildPoint();
	virtual bool checkaddPoint();
	virtual void setillegal( bool illeagal );
	void updateWallDBCoor(float dt);
	virtual bool isNeedShowGrid();
	WallStatus _wallstatus;
	WallCoord* _wallCoord; 
	//是否开始切换动画
	bool       _isBeginPlay;
	RowSelectStatus      _rowStatus;
	vector<UIWall*>      _rowWalls;
    bool                 _isRowH;
	int                  _rowl;
	int                  _rowr;
	bool                 _isRotateRow;
};




#endif



