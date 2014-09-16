#ifndef __MAPHELPER_H__
#define __MAPHELPER_H__

#include <set>
#include <map>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;
/*

*/

//网格坐标
class Coord
{
public:
	int x;
	int	y;
	Coord() {
		x = 0; y = 0;
	}
	Coord(int _x, int _y) {
		x = _x; y = _y;
	}
    bool operator==(const Coord& right);
	bool operator!=(const Coord& right);
	Coord operator-(const Coord& right);
    Coord operator+(const Coord& right);
	Coord& operator-=(const Coord& right);
	Coord& operator+=(const Coord& right);
};


class MapHelper
{
public:
	MapHelper(void);
	~MapHelper(void);

	//void initCoordForUI();
	static void initCoord();//初始化地图坐标点（原点在顶部）
	static void initAngle();
	static float getRotateAngle(const Point & ptAnchor,const Point& first,const Point& second);//求弧度

	static Coord convertPointToCoord(Point point);//将cc坐标转换为网格坐标
	static Point convertCoordToPoint(Coord coord);//将网格坐标转换为cc坐标
	static Point getPointFromCoord(int x,int y); //直接从网格获得坐标
	static bool  checkIsInPlace(int x,int y ,int gridcount);//检查建筑物是否在边界内

	void initBlockPoint();//初始化障碍物点
	void deleteBlockPointByBuilding(int buildingId);//删除障碍物点
	bool isBlockedPoint(int x, int y);//判断是否是障碍物点
private:
	
	void addBlockPoint(int x, int y);
	void deleteBlockPoint(int x, int y);

private:
	map<int, set<int>> m_blockPoint;
};


#endif //__MAPHELPER_H__