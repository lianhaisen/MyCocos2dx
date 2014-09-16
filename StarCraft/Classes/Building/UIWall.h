#ifndef _UIWALL_H
#define _UIWALL_H

#include "UIBuildingBase.h"

//TODO ���BUG

class UIWall;
class WallCoord
{   
public:
	WallCoord(){}
    ~WallCoord();
    int key;//x*MAX_GRID_COUNT+y
	//��ֹ�����ص������
	Vector<UIWall*> walllist;
};

class UIWall : public Building
{
public:
	enum class WallStatus
	{   
		//����
		NOMAL = 1,
		//�ұ�
		RIGHT = 2,
		//���
		LEFT = 3,
		//�м�
		MIDDLE = 4
	};

	enum  class RowSelectStatus
	{  
		//����
		NOMAL = 1,
		//��ѡ��
		ROWSELECTE = 2,
	};

	 UIWall();
	~UIWall();
	 CREATE_FUNC(UIWall);
	 void  releaseWallCoord();
	 void  setWallCoord(Coord coor);
	 RowSelectStatus getRowStatus(){return _rowStatus;}
	 void setRowSelectStatus(RowSelectStatus status){_rowStatus = status;} 
	 //ѡ��ǰ��
	 void getCurrntRow();
	 //��ת��
	 void RotateRow();
private: 
	void  CheckWallStatus(Coord coor);
	void  CheckOtherWall(Coord coor);
	virtual void updateStatus();
	virtual void setMove( bool isMove );
	virtual void setCoor( Coord coor );
	virtual void unselected();
	//�ı��ͷ
	void changeArr();
	//��ԭ��ͷ
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
	//�Ƿ�ʼ�л�����
	bool       _isBeginPlay;
	RowSelectStatus      _rowStatus;
	vector<UIWall*>      _rowWalls;
    bool                 _isRowH;
	int                  _rowl;
	int                  _rowr;
	bool                 _isRotateRow;
};




#endif



