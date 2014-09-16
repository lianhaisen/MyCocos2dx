#include "MapHelper.h"
#include "AppMacros.h"
#include "BuildingData.h"
#include "BuildingConfig.h"
#include "BattleData.h"
#include <math.h>

static double angleP1;
static double angleP2;
static double angleP3;
static double angleP4;
static double width_ui = 0;
static double height_ui = 0;

Point g_coord[MAX_GRID_COUNT + 1][MAX_GRID_COUNT + 1];

MapHelper::MapHelper(void)
{

}

MapHelper::~MapHelper(void)
{
}


float MapHelper::getRotateAngle(const Point & ptAnchor,const Point& first,const Point& second)
{
	float fRt;
	
	double lineP1P2 = sqrt((ptAnchor.x - first.x) * (ptAnchor.x - first.x) + (ptAnchor.y - first.y) * (ptAnchor.y - first.y));
	double lineP1P3 = sqrt((ptAnchor.x - second.x) * (ptAnchor.x - second.x) + (ptAnchor.y - second.y) * (ptAnchor.y - second.y));
	double lineP2P3 = sqrt((first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y));
	fRt = acos((lineP1P2 * lineP1P2 + lineP1P3 * lineP1P3 - lineP2P3 * lineP2P3) / (2 * lineP1P2 * lineP1P3));//* 180.0 / 3.1415926

	return fRt;
}

void MapHelper::initCoord()
{
	for (int i = 0; i <= MAX_GRID_COUNT; i++)  
	{
		for (int j = 0; j <= MAX_GRID_COUNT; j++)
		{
			g_coord[i][j] = Point(POINT1.x - (i - j) * UNIT_WIDTH, POINT1.y - (i + j) * UNIT_HEIGHT);
		}
	}
}


void MapHelper::initAngle()
{
	angleP1 = getRotateAngle(POINT1, POINT2, POINT4);
	angleP2 = getRotateAngle(POINT2, POINT1, POINT4);
	angleP3 = getRotateAngle(POINT4, POINT2, POINT1);
	angleP4 = getRotateAngle(POINT4, POINT3, POINT1);
	double disx = sqrt((POINT2.x - POINT1.x) * (POINT2.x - POINT1.x) + (POINT2.y - POINT1.y) * (POINT2.y - POINT1.y));
	double disy = sqrt((POINT4.x - POINT1.x) * (POINT4.x - POINT1.x) + (POINT4.y - POINT1.y) * (POINT4.y - POINT1.y));
	width_ui = disx/MAX_GRID_COUNT;
	height_ui = disy/MAX_GRID_COUNT;
}

Coord MapHelper::convertPointToCoord(Point point)
{
	if (width_ui == 0 && height_ui == 0)
	{
		initAngle();
	}
	double angle = getRotateAngle(POINT1,point, POINT2);
	double agelet = angle * 180.0 / 3.1415926;
	double dis = sqrt((point.x - POINT1.x) * (point.x - POINT1.x) + (point.y - POINT1.y) * (point.y - POINT1.y));
	double sinv;
	double cosv;
	double ad;
	double bd;
	double xlen;
	double ylen;
	if(agelet>90.0)
	{
		sinv = sin(angle);
		ad = dis*sinv;
		bd = sqrt(dis*dis-ad*ad);
		
		ylen = ad/sin(angleP4);
		xlen = sqrt(ylen*ylen-ad*ad) - bd;
	}
	else
	{
		sinv = sin(angle);
		cosv = cos(angle);
		ad = dis*sinv;
		bd = dis*cosv;
		ylen=dis*sinv/sin(angleP4);
		double cd = sqrt(ylen*ylen-ad*ad);
		xlen = bd+cd;//cos(angle)*dis/sin(angleP3);
	}
	
	//float ylen = cos(angle) + (asin(angle)/tan(angleP3));
	Coord coord;
	coord.x = (int)xlen/width_ui+1;
	coord.y = (int)ylen/height_ui+1;
	return coord;
}

Point MapHelper::convertCoordToPoint(Coord coord)
{
	Point point = Point(0, 0);
	if (coord.x < MAX_GRID_COUNT && coord.y < MAX_GRID_COUNT)
	{
		point = Point(g_coord[coord.x][coord.y]);
	}
	else
	{
		//TODO 处理越界情况
	}
	return point;
}


//TODO:地面部队可以踩在建筑物的草皮（间隙）穿过去
void MapHelper::initBlockPoint()
{
	auto bds = BattleData::getInstance()->getDefendBuilding();
	for (auto item : bds)
	{
		auto bd = item.second;
		CBuilding *bc = BuildingConfig::getBuildingConfig(bd->getType());
		for (int i = 0; i < bc->grid - 1; i++)
		{
			for (int j = 0; j < bc->grid - 1; j++)
			{
				// TODO x, y做越界判断
				addBlockPoint(bd->getCoord().x - i, bd->getCoord().y - j);
			}
		}
	}
}

void MapHelper::deleteBlockPointByBuilding(int buildingId)
{
	DBuilding *bd = BuildingData::getBuildingData(buildingId);
	CBuilding *bc = BuildingConfig::getBuildingConfig(bd->type);

	for (int i = 0; i < bc->grid - 1; i++)
	{
		for (int j = 0; j < bc->grid - 1; j++)
		{
			deleteBlockPoint(bd->coord.x - i, bd->coord.y - j);
		}
	}
}

void MapHelper::addBlockPoint(int x, int y)
{
	map<int, set<int>>::iterator itx = m_blockPoint.find(x);
	if (itx != m_blockPoint.end())
	{
		set<int>& yset = itx->second;
		yset.insert(y);
	} 
	else
	{
		set<int> s;
		s.insert(y);
		m_blockPoint[x] = s;
	}
}

void MapHelper::deleteBlockPoint(int x, int y)
{
	map<int, set<int>>::iterator itx = m_blockPoint.find(x);
	if (itx != m_blockPoint.end())
	{
		set<int>& yset = itx->second;
		yset.erase(y);

		if (yset.size() == 0)
		{
			m_blockPoint.erase(x);
		}
	} 
}

bool MapHelper::isBlockedPoint(int x, int y)
{
	map<int, set<int>>::iterator itx = m_blockPoint.find(x);
	if (itx != m_blockPoint.end())
	{
		set<int>& yset = itx->second;
		set<int>::iterator ity = yset.find(y);
		if (ity != yset.end())
		{
			return true;
		}
	}
	return false;
}

Point MapHelper::getPointFromCoord( int x,int y )
{
	//TODO: is right?
	return Point(POINT1.x - (x - y) * UNIT_WIDTH, POINT1.y - (x + y) * UNIT_HEIGHT);
}

bool MapHelper::checkIsInPlace( int cx,int cy ,int gridcount )
{
	//TODO: is right?
	if (cx - gridcount >=0 && cx <= MAX_GRID_COUNT && cy - gridcount >=0 && cy <= MAX_GRID_COUNT)
	{
		return true;
	}
	return false;
}



bool Coord::operator==( const Coord& right )
{
	return this->x == right.x && this->y == right.y;
}

bool Coord::operator!=( const Coord& right )
{
	return this->x != right.x || this->y != right.y;
}

Coord Coord::operator-( const Coord& right )
{
	return Coord(this->x - right.x,this->y - right.y);
}

Coord Coord::operator+( const Coord& right )
{
	return Coord(this->x + right.x,this->y + right.y);
}

Coord& Coord::operator-=( const Coord& right )
{
	this->x -= right.x;
	this->y -= right.y;
	return *this;
}

Coord& Coord::operator+=( const Coord& right )
{   
	this->x += right.x;
	this->y += right.y;
	return *this;
}
