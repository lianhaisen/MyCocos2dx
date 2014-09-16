/** 
* @file         AStar.h 
* @Synopsis     AStar algorithm 
* @author       xiaoming(c265n46@163.com) 
* @version      1.0 
* @date         2012-11-15  
*/

/************************************************************************/
/* f(n) = g(n) + h(n)
/* g(n) 为当前点到起点的移动距离度量
/* h(n) 为当前点到终点的移动距离度量
/************************************************************************/

#ifndef _A_STAR_H_
#define _A_STAR_H_

#include <list>
#include "MapHelper.h"

//改用MapHelper的Coord
//struct point
//{
//	unsigned int x, y;
//	point() {
//		x = 0; y = 0;
//	}
//	point(unsigned int _x, unsigned int _y) {
//		x = _x; y = _y;
//	}
//};

struct ANode
{
	Coord coord;//地图坐标点
	unsigned int f, g, h;
	ANode *parent;//用于寻路时指定父节点
	ANode *next;//用于ANode链表
};

class ANodePool
{
public:
	ANodePool();
	virtual ~ANodePool();

	static ANode* allocNode();
	static void freeNode(ANode *node);
	static void freeAll();
private:
	static ANode *m_freeList;
};

class AStar
{
public:
	AStar();
	virtual ~AStar();

	void setStartPoint(const Coord& coord);//设置起点
	void setEndPoint(const Coord& coord);//设置终点
	void startSearch();//开始最短路径搜索
	void setMap(MapHelper *m_pMap);//设置地图
	void getPathList(std::list<Coord> &path);
	void getPath(std::vector<Coord> &path);
	void printStep();
private:
	bool isInList(const ANode* node, int list_type);//判断是否在节点链表
	void addToCloseList(ANode* node);//加入close节点链表
	void addToOpenList(ANode* node);//加入open节点链表
	void validAddToOpenList(ANode* node);//验证点的有效性并加入open节点链表
	void setGn(ANode* node);//起始点到当前点的移动量
	void setHn(ANode* node);//当前点到目标点的移动量
	void setFn(ANode* node);//设置f的值 f(n) = g(n) + h(n)
	ANode* getBestOpenNode();//从open节点链表中获取f值最小的那个
	ANode* getFromList(const unsigned int x, const unsigned int y, int list_type);//从链表中取出指定坐标的节点
	bool isSearchFinished(ANode *node);//判断寻路是否结束
	bool isBlocked(const unsigned int x, const unsigned int y);//判断是否阻挡点
	void removeFromOpenList(ANode* node);//移除open节点列表中的node节点

private:
	MapHelper *m_pMap;//地图
	int m_iStep;
	Coord m_start;//起点
	Coord m_end;//终点
	ANode* m_openList;//open节点链表
	ANode* m_closeList;//close节点链表
	static int m_xoff[8];//记录周边8个点的x坐标偏移
	static int m_yoff[8];//记录周边8个点的y坐标偏移
	static const unsigned int H_FAC = 10; //h的权值
	static const unsigned int G_FORWARD_FAC = 10; //正向g的权值
	static const unsigned int G_CROSS_FAC = 14; //斜向g的权值

	enum LIST_TYPE {//指定要操作的链表类型
		OPEN,
		CLOSE
	};
};

#endif