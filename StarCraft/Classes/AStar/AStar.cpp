/** 
* @file         AStar.cpp 
* @Synopsis      
* @author       xiaoming(c265n46@163.com) 
* @version      1.0 
* @date         2012-11-15  
*/
#include "AStar.h"
//#include "map_handler.h"

ANode* ANodePool::m_freeList = NULL;

int AStar::m_xoff[8] = {0, 1, 1, -1,  0, -1,  1, -1};
int AStar::m_yoff[8] = {1, 1, 0,  0, -1, -1, -1,  1};

void ANodePool::freeAll()
{
	ANode *pNode = NULL;
	while (NULL != m_freeList) {
		pNode = m_freeList;
		m_freeList = m_freeList->next;
		delete pNode;
	}
}

ANode* ANodePool::allocNode()
{
	ANode *pNode = NULL;
	if (NULL != m_freeList) {
		pNode = m_freeList;
		m_freeList = m_freeList->next;
	} else {
		pNode = new ANode;
	}

	memset(pNode, 0, sizeof(ANode));
	return pNode;
}

void ANodePool::freeNode(ANode *node)
{
	ANode *pNode = m_freeList;
	m_freeList = node;
	m_freeList->next = pNode;
}

AStar::AStar()
{
	m_pMap = NULL;
	m_openList = NULL;
	m_closeList = NULL;
}

AStar::~AStar()
{
	ANode* pNode = NULL;

	while (NULL != m_openList) {
		pNode = m_openList;
		m_openList = m_openList->next;
		delete pNode;
	}

	while (NULL != m_closeList) {
		pNode = m_closeList;
		m_closeList = m_closeList->next;
		delete pNode;
	}

	ANodePool::freeAll();
}

void AStar::setStartPoint(const Coord& coord)
{
	m_start.x = coord.x;
	m_start.y = coord.y;
}

void AStar::setEndPoint(const Coord& coord)
{
	m_end.x = coord.x;
	m_end.y = coord.y;
}

void AStar::startSearch()
{
	if (m_start.x == m_end.x && m_start.y == m_end.y) {
		return;
	}

	ANode *bestNode = ANodePool::allocNode();
	bestNode->g = 0;
	setHn(bestNode);
	setFn(bestNode);
	bestNode->parent = NULL;
	bestNode->coord.x = m_start.x;
	bestNode->coord.y = m_start.y;

	ANode *pNearlyNode = NULL;
	m_iStep = 0;
	while (!isSearchFinished(bestNode)) {
		//处理bestNode附近的8个节点
		m_iStep++;
		for (int i = 0; i < 8; i++) {
			pNearlyNode = ANodePool::allocNode();
			pNearlyNode->parent = bestNode;
			pNearlyNode->coord.x = bestNode->coord.x + m_xoff[i];
			pNearlyNode->coord.y = bestNode->coord.y + m_yoff[i];
			validAddToOpenList(pNearlyNode);
		}

		addToCloseList(bestNode);
		bestNode = getBestOpenNode();
	}

	return;
}

bool AStar::isInList(const ANode* node, int list_type)
{
	ANode* pNode = NULL;
	if (OPEN == list_type) {
		pNode = m_openList;
	}

	if (CLOSE == list_type) {
		pNode = m_closeList;
	}

	while (NULL != pNode) {
		if (pNode->coord.x == node->coord.x && pNode->coord.y == node->coord.y) {
			return true;
		}
		pNode = pNode->next;  
	}

	return false;
}

void AStar::addToCloseList(ANode* node)
{
	if (NULL == node) {
		return;
	}

	if (NULL == m_closeList) {
		m_closeList = node;
		m_closeList->next = NULL;
		return;
	}

	ANode* pNode = m_closeList;
	m_closeList = node;
	m_closeList->next = pNode;

}

void AStar::addToOpenList(ANode* node)
{
	if (NULL == m_openList) {
		m_openList = node;
		m_openList->next = NULL;
		return;
	}

	//找到第一个fn比它大的节点并插到它前面
	//以保证该链表有序
	ANode* pPre = NULL;
	ANode* pNode = m_openList;
	while (NULL != pNode) {
		if (pNode->f >= node->f) {
			if (NULL == pPre) {
				node->next = pNode;
				m_openList = node;
				break;
			}
			pPre->next = node;
			node->next = pNode;
			break;
		}

		pPre = pNode;
		pNode = pNode->next;
	}

	//没有找到位置
	if (NULL == pNode) {
		pPre->next = node;
		node->next = NULL;
	}
}

void AStar::setGn(ANode* node)
{
	if ((node->coord.x != node->parent->coord.x) && (node->coord.y != node->parent->coord.y)) {
		//是对角点
		node->g = G_CROSS_FAC + node->parent->g;
	} else {
		node->g = G_FORWARD_FAC + node->parent->g;
	}
}

void AStar::setHn(ANode* node)
{
	node->h = (abs(int(node->coord.x - m_end.x)) + abs(int(node->coord.y - m_end.y))) * H_FAC;
}

void AStar::setFn(ANode* node)
{
	//请注意顺序，先设置好g(n)和h(n)
	node->f = node->g + node->h;
}

ANode* AStar::getBestOpenNode()
{
	if (NULL == m_openList) {
		return NULL;
	}

	//找出fn最小的node
	//由于open节点链表本身有序，所以只需要返回头节点
	ANode *pNode = m_openList;
	m_openList = m_openList->next;
	pNode->next = NULL;//这个节点将被加入到close节点链表
	return pNode;
}

bool AStar::isSearchFinished(ANode *node)
{
	//如果BestNode节点与m_end坐标相同，则寻路结束
	if ((m_end.x == node->coord.x) && (m_end.y == node->coord.y)) {
		addToCloseList(node);
		return true;
	}
	return false;
}

bool AStar::isBlocked(const unsigned int x, const unsigned int y)
{
	if (NULL == m_pMap) {
		return true;
	}

	return m_pMap->isBlockedPoint(x, y);
}

ANode* AStar::getFromList(const unsigned int x, const unsigned int y, int list_type)
{
	ANode *pNode = NULL;
	if (OPEN == list_type) {
		pNode = m_openList;
	}

	if (CLOSE == list_type) {
		pNode = m_closeList;
	}

	while (NULL != pNode) {
		if (pNode->coord.x == x && pNode->coord.y == y) {
			return pNode;
		}
		pNode = pNode->next;
	}

	return pNode;
}

void AStar::validAddToOpenList(ANode* node)
{
	//节点超出地图范围或者是阻挡点
	if (isBlocked(node->coord.x, node->coord.y)) {
		ANodePool::freeNode(node);
		return;
	}

	setGn(node);//这里要注意顺序哦
	setHn(node);
	setFn(node);

	//节点在close链表中
	ANode *pNode = getFromList(node->coord.x, node->coord.y, CLOSE);
	if (NULL != pNode) {
		//取node的g(n)并与已经存在于链表中的同一个点的g(n)比较
		//取2者g(n)的较小值
		if (pNode->g > node->g) {
			pNode->g = node->g;
			pNode->f = node->f;
			pNode->h = node->h;
			pNode->parent = node->parent;
		}

		ANodePool::freeNode(node);
		return;
	}

	//节点已经存在于open链表中
	pNode = getFromList(node->coord.x, node->coord.y, OPEN);
	if (NULL != pNode) {
		//取node的g(n)并与已经存在于链表中的同一个点的g(n)比较
		//取2者g(n)的较小值
		if ((pNode->g) > (node->g)) {
			pNode->f = node->f;
			pNode->g = node->g;
			pNode->h = node->h;
			pNode->parent = node->parent;

			//节点的g(n)值发生变化，需要调整它在open节点链表中的位置
			removeFromOpenList(pNode);
			addToOpenList(pNode);
		}

		ANodePool::freeNode(node);
		return;
	} 

	addToOpenList(node);
}

void AStar::setMap(MapHelper *_map)
{
	m_pMap = _map;
}

void AStar::getPathList(std::list<Coord> &path)
{
	ANode *pNode = NULL;
	pNode = getFromList(m_end.x, m_end.y, CLOSE);

	while (pNode != NULL) {
		path.push_back(Coord(pNode->coord.x, pNode->coord.y));
		pNode = pNode->parent;
	}
}

void AStar::getPath(std::vector<Coord> &path)
{
	ANode *pNode = NULL;
	pNode = getFromList(m_end.x, m_end.y, CLOSE);

	while (pNode != NULL) {
		path.push_back(Coord(pNode->coord.x, pNode->coord.y));
		pNode = pNode->parent;
	}
}

void AStar::removeFromOpenList(ANode* node)
{
	ANode *pPre = NULL;
	ANode *pNode = m_openList;
	while (NULL != pNode) {
		if (pNode->coord.x == node->coord.x && pNode->coord.y == node->coord.y) {
			if (NULL == pPre) {
				m_openList = m_openList->next;
				pNode->next = NULL;
				break;
			}
			pPre->next = pNode->next;
			pNode->next = NULL;
			break;
		}
		pPre = pNode;
		pNode = pNode->next;
	}

	return ;
}


void AStar::printStep()
{
	printf("\ntakes %d step to finish\n", m_iStep);
}