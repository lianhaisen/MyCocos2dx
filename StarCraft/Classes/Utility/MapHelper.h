#ifndef __MAPHELPER_H__
#define __MAPHELPER_H__

#include <set>
#include <map>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;
/*

*/

//��������
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
	static void initCoord();//��ʼ����ͼ����㣨ԭ���ڶ�����
	static void initAngle();
	static float getRotateAngle(const Point & ptAnchor,const Point& first,const Point& second);//�󻡶�

	static Coord convertPointToCoord(Point point);//��cc����ת��Ϊ��������
	static Point convertCoordToPoint(Coord coord);//����������ת��Ϊcc����
	static Point getPointFromCoord(int x,int y); //ֱ�Ӵ�����������
	static bool  checkIsInPlace(int x,int y ,int gridcount);//��齨�����Ƿ��ڱ߽���

	void initBlockPoint();//��ʼ���ϰ����
	void deleteBlockPointByBuilding(int buildingId);//ɾ���ϰ����
	bool isBlockedPoint(int x, int y);//�ж��Ƿ����ϰ����
private:
	
	void addBlockPoint(int x, int y);
	void deleteBlockPoint(int x, int y);

private:
	map<int, set<int>> m_blockPoint;
};


#endif //__MAPHELPER_H__