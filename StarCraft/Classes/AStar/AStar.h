/** 
* @file         AStar.h 
* @Synopsis     AStar algorithm 
* @author       xiaoming(c265n46@163.com) 
* @version      1.0 
* @date         2012-11-15  
*/

/************************************************************************/
/* f(n) = g(n) + h(n)
/* g(n) Ϊ��ǰ�㵽�����ƶ��������
/* h(n) Ϊ��ǰ�㵽�յ���ƶ��������
/************************************************************************/

#ifndef _A_STAR_H_
#define _A_STAR_H_

#include <list>
#include "MapHelper.h"

//����MapHelper��Coord
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
	Coord coord;//��ͼ�����
	unsigned int f, g, h;
	ANode *parent;//����Ѱ·ʱָ�����ڵ�
	ANode *next;//����ANode����
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

	void setStartPoint(const Coord& coord);//�������
	void setEndPoint(const Coord& coord);//�����յ�
	void startSearch();//��ʼ���·������
	void setMap(MapHelper *m_pMap);//���õ�ͼ
	void getPathList(std::list<Coord> &path);
	void getPath(std::vector<Coord> &path);
	void printStep();
private:
	bool isInList(const ANode* node, int list_type);//�ж��Ƿ��ڽڵ�����
	void addToCloseList(ANode* node);//����close�ڵ�����
	void addToOpenList(ANode* node);//����open�ڵ�����
	void validAddToOpenList(ANode* node);//��֤�����Ч�Բ�����open�ڵ�����
	void setGn(ANode* node);//��ʼ�㵽��ǰ����ƶ���
	void setHn(ANode* node);//��ǰ�㵽Ŀ�����ƶ���
	void setFn(ANode* node);//����f��ֵ f(n) = g(n) + h(n)
	ANode* getBestOpenNode();//��open�ڵ������л�ȡfֵ��С���Ǹ�
	ANode* getFromList(const unsigned int x, const unsigned int y, int list_type);//��������ȡ��ָ������Ľڵ�
	bool isSearchFinished(ANode *node);//�ж�Ѱ·�Ƿ����
	bool isBlocked(const unsigned int x, const unsigned int y);//�ж��Ƿ��赲��
	void removeFromOpenList(ANode* node);//�Ƴ�open�ڵ��б��е�node�ڵ�

private:
	MapHelper *m_pMap;//��ͼ
	int m_iStep;
	Coord m_start;//���
	Coord m_end;//�յ�
	ANode* m_openList;//open�ڵ�����
	ANode* m_closeList;//close�ڵ�����
	static int m_xoff[8];//��¼�ܱ�8�����x����ƫ��
	static int m_yoff[8];//��¼�ܱ�8�����y����ƫ��
	static const unsigned int H_FAC = 10; //h��Ȩֵ
	static const unsigned int G_FORWARD_FAC = 10; //����g��Ȩֵ
	static const unsigned int G_CROSS_FAC = 14; //б��g��Ȩֵ

	enum LIST_TYPE {//ָ��Ҫ��������������
		OPEN,
		CLOSE
	};
};

#endif