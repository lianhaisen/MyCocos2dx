#include "UIWall.h"
#include "BuildingCommon.h"
#include "UIPlaceBuildingLayer.h"

WallCoord::~WallCoord()
{
	walllist.clear();
}
UIWall::UIWall()
{
	_wallstatus = WallStatus::NOMAL;
	_wallCoord = nullptr;
	_isBeginPlay = false;
	_rowStatus = RowSelectStatus::NOMAL;
	_isRowH = false;
	_rowl = 0;
	_rowr =0;
	_isRotateRow = false;
}
UIWall::~UIWall()
{

}

void UIWall::updateStatus()
{    
    getBuildingArmature()->getAnimation()->play(BuildingCommon::getWallStr(getLevel(),(int)_wallstatus),-1,0);
}

void UIWall::setCoor( Coord coor )
{   

	auto pre = getCoor();
	if (_rowStatus == RowSelectStatus::NOMAL)
	{
	
		Building::setCoor(coor);
		//第一次移动改变位置
		if ( coor != pre && getIsMove())
		{    
			if (!_isBeginPlay)
			{
				_isBeginPlay = true;
				releaseWallCoord();
				_wallstatus = WallStatus::NOMAL;
				updateStatus();
				CheckOtherWall(pre);
			}		
		}
	}
	else if (_rowStatus == RowSelectStatus::ROWSELECTE && !_rowWalls.empty())
	{   
		
	    if (coor != pre)
		{   
			auto diffcoor = coor - pre;
			for (auto item : _rowWalls)
			{
				item->_coor += diffcoor;
				auto pos = MapHelper::getPointFromCoord(item->_coor.x,item->_coor.y);
				item->setPosition(pos);
			}
		}
	}
	
}

void UIWall::releaseWallCoord()
{
	if (_wallCoord)
	{
		for (auto i = _wallCoord->walllist.begin(); i != _wallCoord->walllist.end(); i++)
		{
			if (*i == this)
			{
				_wallCoord->walllist.erase(i);
				break;
			}
		}
		_wallCoord = nullptr;
	}
}
void UIWall::setWallCoord( Coord coor )
{
	auto layer = getPlaceLayer();
	if (layer)
	{
		if (_wallCoord != layer->getWallCoord(coor))
		{
            releaseWallCoord();
			_wallCoord = layer->getWallCoord(coor);
			_wallCoord->walllist.pushBack(this);
			CheckWallStatus(coor);
			CheckOtherWall(coor);
		}
	}
}

void UIWall::CheckWallStatus(Coord coor)
{
	auto coor1 = Coord(coor.x,coor.y - 2);
	auto coor2 = Coord(coor.x-2,coor.y);
	auto layer =  getPlaceLayer();
	if (layer)
	{
		auto wall1 = layer->getWallFromCoord(coor1);
		auto wall2 = layer->getWallFromCoord(coor2);
		if (!wall1 && !wall2)
		{
			_wallstatus = WallStatus::NOMAL;
		}
		else if (wall1 && !wall2)
		{
			_wallstatus = WallStatus::LEFT;
		}
		else if ( !wall1 && wall2)
		{
			_wallstatus = WallStatus::RIGHT;
		}
		else
		{
			_wallstatus = WallStatus::MIDDLE;
		}
		updateStatus();

	}
}

void UIWall::CheckOtherWall( Coord coor)
{
	auto coor1 = Coord(coor.x,coor.y - 2);
	auto coor2 = Coord(coor.x,coor.y + 2);
	auto coor3 = Coord(coor.x-2,coor.y);
	auto coor4 = Coord(coor.x+2,coor.y);
	auto layer =  getPlaceLayer();
	if (layer)
	{
		auto wall = layer->getWallFromCoord(coor1);
		if (wall )
		{ 
			wall->CheckWallStatus(wall->getCoor());
		}
		wall =  layer->getWallFromCoord(coor2);
		if (wall )
		{ 
			wall->CheckWallStatus(wall->getCoor());
		}
		wall =  layer->getWallFromCoord(coor3);
		if (wall )
		{ 
			wall->CheckWallStatus(wall->getCoor());
		}
		wall =  layer->getWallFromCoord(coor4);
		if (wall )
		{ 
			wall->CheckWallStatus(wall->getCoor());
		}
	}
}

void UIWall::setMove( bool isMove )
{  
	if (_rowStatus == RowSelectStatus::NOMAL)
	{  
		Building::setMove(isMove);
		if (!isMove)
		{
			_isBeginPlay = false;
			auto coor = getCoor();
			setWallCoord(coor);
		}
		return;
	}
	else if (_rowStatus == RowSelectStatus::ROWSELECTE)
	{
		_bMove = isMove;
		if (!isMove)
		{
			if (!this->_isillegal)
			{
				unselected();
			}
		}
	}

}

void UIWall::getCurrntRow()
{   
	if (_rowStatus != RowSelectStatus::NOMAL)
	{
		return;
	}
	auto layer = getPlaceLayer();
	if (!layer)
	{
		return;
	}
	_rowWalls.clear();
	int lx = 0;
	int rx = 0;
	int ly = 0;
	int ry = 0;

	auto curcoor = getCoor();
	while (layer->getWallFromCoord(Coord(curcoor.x - 2,curcoor.y)) != nullptr)
	{
	    rx++;
		curcoor = Coord(curcoor.x - 2,curcoor.y);
	}

	curcoor = getCoor();
	while (layer->getWallFromCoord(Coord(curcoor.x + 2,curcoor.y)) != nullptr)
	{
		lx++;
		curcoor = Coord(curcoor.x + 2,curcoor.y);
	}

	curcoor = getCoor();
	while (layer->getWallFromCoord(Coord(curcoor.x,curcoor.y - 2)) != nullptr)
	{
		ry++;
		curcoor = Coord(curcoor.x,curcoor.y - 2);
	}

	curcoor = getCoor();
	while (layer->getWallFromCoord(Coord(curcoor.x,curcoor.y + 2)) != nullptr)
	{
		ly++;
		curcoor = Coord(curcoor.x,curcoor.y + 2);
	}

	if ((lx + rx) == 0 && (ry + ly) == 0)
	{
		return;
	}
	if ((lx + rx) >= (ry + ly))
	{   
		_isRowH = true;
		_rowl = lx;
		_rowr = rx;
		for (int i = -rx; i <= lx; i++)
		{
			curcoor = Coord(getCoor().x + i * 2,getCoor().y);
			auto wall = layer->getWallFromCoord(curcoor);
			_rowWalls.push_back(wall);
		}
	}
	else
	{   
		_isRowH = false;
		_rowl = ly;
		_rowr = ry;
		for (int i = -ry; i <= ly; i++)
		{   
			int index = getCoord().y ;
			curcoor = Coord(getCoor().x ,getCoor().y + i*2);
			auto wall = layer->getWallFromCoord(curcoor);
			_rowWalls.push_back(wall);
		}
	}

	int index = 0;
	for(auto item : _rowWalls)
	{    
		item->_orginCoor = item->getCoor();
		item->setRowSelectStatus(RowSelectStatus::ROWSELECTE);
		item->setLocalZOrder(item->getCoor().y * MAX_GRID_COUNT + item->getCoor().x + 100000);
		item->releaseWallCoord();
		if (index == 0)
		{
			item->_wallstatus = WallStatus::NOMAL;
		}
		else
		{
			if (_isRowH)
			{
				item->_wallstatus = WallStatus::RIGHT;
			}
			else
			{
				item->_wallstatus = WallStatus::LEFT;
			}
		}
		item->updateStatus();
		item->CheckOtherWall(item->getCoor());
		index++;
	}
	changeArr();

}

void UIWall::unselected()
{  

	if (_rowStatus == RowSelectStatus::NOMAL)
	{
		Building::unselected();
		return;
	}
	else if (_rowStatus == RowSelectStatus::ROWSELECTE)
	{   
		if (_Selected)
		{   
			_Selected = false;
			getArr1()->setVisible(false);
			getArr2()->setVisible(false);
			getArr3()->setVisible(false);
			getArr4()->setVisible(false);
			getBuildingArmature()->stopAction(getSharkAction());
			getBuildingArmature()->setColor(Color3B(255,255,255));
			if (_callBack)
			{
				_callBack(this);
			}
		}

		auto diff = _orginCoor - _coor;
		for (auto item :_rowWalls)
		{   
			item->_isBeginPlay = false;
			if (item->_isillegal)
			{
				if (_isRotateRow)
				{
					item->_coor = item->_orginCoor;
				}
				else
				{
					item->_coor += diff;
				}
				auto pos = MapHelper::getPointFromCoord(item->_coor.x,item->_coor.y);
				item->setPosition(pos);
				item->_isillegal = false;
			}
			item->setRowSelectStatus(RowSelectStatus::NOMAL);
			item->setLocalZOrder(item->getCoor().y * MAX_GRID_COUNT + item->getCoor().x);
			auto coor = item->getCoor();
			item->setWallCoord(coor);
			auto layer = getPlaceLayer();
			if (layer)
			{
				layer->addbuildPoint(coor.x,coor.y,m_nGridCount);
			}
		}
	    this->schedule(schedule_selector( UIWall::updateWallDBCoor),0.1f);
		_rowStatus = RowSelectStatus::NOMAL;
		_isRotateRow = false;
		resertArr();

		
	}
}


void UIWall::changeArr()
{   
	if (_rowStatus != RowSelectStatus::ROWSELECTE)
	{
		return;
	}
	Size size = getContentSize();
	Point p1 = Point(size.width/2, size.height);
	Point p2 = Point(0, size.height/2);
	float w = (p1.x-p2.x)/m_nGridCount;
	float h = (p1.y-p2.y)/m_nGridCount;
	if (_isRowH)
	{
		auto a1 = getArr1()->getPosition();
		getArr1()->setPosition(a1 + Point(_rowr * w,_rowr * h));
		auto a4 = getArr4()->getPosition();
		getArr4()->setPosition(a4 + Point(- _rowl * w,- _rowl*h));
		float count = (float)(_rowr - _rowl)/2;
		auto a2 = getArr2()->getPosition();
		getArr2()->setPosition(a2 + Point(count * w,count * h));
		auto a3 = getArr3()->getPosition();
		getArr3()->setPosition(a3 + Point(count * w,count * h));
	}
	else
	{
		auto a2 = getArr2()->getPosition();
		getArr2()->setPosition(a2 + Point(-_rowr * w,_rowr*h));
		auto a3 = getArr3()->getPosition();
		getArr3()->setPosition(a3 + Point(_rowl * w, -_rowl * h));
		float count = (float)(_rowr - _rowl)/2;
		auto a1 = getArr1()->getPosition();
		getArr1()->setPosition(a1 + Point(-count * w,count * h)); 
		auto a4 = getArr4()->getPosition();
		getArr4()->setPosition(a4 + Point(-count * w,count * h)); 
	}

	
}

void UIWall::resertArr()
{
	Size arrsize = getArr1()->getContentSize();
	getArr1()->setPosition(Point(getContentSize().width / 4 * 3 + arrsize.width / 4,getContentSize().height / 4 * 3 +  arrsize.height /4 ));
	getArr2()->setPosition(Point(getContentSize().width / 4 - arrsize.width / 4,getContentSize().height / 4 * 3 +  arrsize.height /4 ));
	getArr3()->setPosition(Point(getContentSize().width  / 4 * 3 + arrsize.width / 4 ,getContentSize().height / 4 - arrsize.height /4 ));
	getArr4()->setPosition(Point(getContentSize().width / 4 - arrsize.width / 4 ,getContentSize().height / 4 - arrsize.height /4));
}

bool UIWall::isPointInRect( const Point& point )
{   
	
	bool result = false;
	if (_rowStatus == RowSelectStatus::NOMAL)
	{   
		result = UIBuildingBase::isPointInRect(point);
		return result;
	}
	if (_rowWalls.empty())
	{
		return result;
	}

	for (auto item : _rowWalls)
	{
		Rect r = item->getBoundingBox();
		if (r.containsPoint(point))
		{  
			result = true;
			break;
		}
	}
	return result;

}

void UIWall::addBuildPoint()
{
	if (_rowStatus == RowSelectStatus::NOMAL)
	{   
		UIBuildingBase::addBuildPoint();
		return;
	}
	auto layer = getPlaceLayer();
	if (layer)
	{
		for (auto item : _rowWalls)
		{
			layer->addbuildPoint(item->getCoor().x,item->getCoor().y,item->getGridCount());
		}
	}
	
}

void UIWall::removeBuildPoint()
{
	if (_rowStatus == RowSelectStatus::NOMAL)
	{   
		Building::removeBuildPoint();
		return;
	}
	else if (_rowStatus == RowSelectStatus::ROWSELECTE)
	{
		auto layer = getPlaceLayer();
		if (layer && !_isillegal)
		{
			for (auto item : _rowWalls)
			{
				layer->removebuildingPoint(item->getCoor().x,item->getCoor().y,item->getGridCount());
			}
		}
	}
	
}

bool UIWall::checkaddPoint()
{
	if (_rowStatus == RowSelectStatus::NOMAL)
	{
		return UIBuildingBase::checkaddPoint();
	}
	auto result = true;
	auto layer = getPlaceLayer();
	if (layer)
	{
		for (auto item : _rowWalls)
		{
			if (!layer->checkaddPoint(item->getCoor().x,item->getCoor().y,item->getGridCount()))
			{   
				result = false;
				break;
			}
		}
	}
	return result;
}

void UIWall::setillegal( bool illeagal )
{
	if (_rowStatus == RowSelectStatus::NOMAL)
	{
		 Building::setillegal(illeagal);
		 return;
	}
	else if(_rowStatus == RowSelectStatus::ROWSELECTE )
	{
		for (auto item : _rowWalls)
		{
			item->_isillegal = illeagal;
		}	
	}
	

}

void UIWall::updateWallDBCoor( float dt )
{
	if(!_rowWalls.empty())
	{   
		for(auto item : _rowWalls)
		{
			auto db = item->getBuidingData();
			if (db && db->coord != item->getCoor())
			{
				db->updateCoor(item->getCoor().x,item->getCoor().y);
			}
		}
		_rowWalls.clear();
	}
    this->unschedule(schedule_selector( UIWall::updateWallDBCoor));
}

void UIWall::RotateRow()
{
	if (_rowStatus != RowSelectStatus::ROWSELECTE)
	{
		return;
	}
	auto layer = getPlaceLayer();
	if (!layer)
	{
		return;
	}
	//TODO 记录合理的位置
	//第一次移除判断点
	//改变坐标
	//检查坐标是否合理
	if (!_isRotateRow)
	{
		this->removeBuildPoint();
	}
	
	for (auto item : _rowWalls)
	{   
		if (!_isRotateRow && !item->_isillegal)
		{
			layer->removebuildingPoint(item->getCoor().x,item->getCoor().y,m_nGridCount);
			item->_orginCoor = item->getCoor();
		}
		auto diff = item->getCoor() - _coor;
		if (_isRowH)
		{
			diff = Coord(0 - diff.y,0 - diff.x);
			item->_coor = _coor + diff;
			auto pos = MapHelper::getPointFromCoord(item->_coor.x,item->_coor.y);
			item->setPosition(pos);
		}
		else
		{
			diff = Coord(diff.y,diff.x);
			item->_coor = _coor + diff;
			auto pos = MapHelper::getPointFromCoord(item->_coor.x,item->_coor.y);
			item->setPosition(pos);
		}	
	}

	if (_isRowH)
	{
		auto tmp = _rowl;
		_rowl =_rowr;
		_rowr = tmp;

		vector<UIWall*> tmpvc;
		for (auto it = _rowWalls.rbegin(); it != _rowWalls.rend();++it)
		{
			tmpvc.push_back(*it);
		}
	    _rowWalls = tmpvc;
	}
    _isRowH = !_isRowH;

	int index = 0;
	for(auto item : _rowWalls)
	{
		item->setLocalZOrder(item->getCoor().y * MAX_GRID_COUNT + item->getCoor().x + 100000);
		if (index == 0)
		{
			item->_wallstatus = WallStatus::NOMAL;
		}
		else
		{
			if (_isRowH)
			{
				item->_wallstatus = WallStatus::RIGHT;
			}
			else
			{
				item->_wallstatus = WallStatus::LEFT;
			}
		}
		item->updateStatus();
		index++;
	}

	resertArr();
	changeArr();
	setillegal(!checkaddPoint());
	_isRotateRow = true;
}

bool UIWall::isNeedShowGrid()
{
	if (_rowStatus == RowSelectStatus::ROWSELECTE)
	{
		return true;
	}
	else
	{
		return Building::isNeedShowGrid();
	}
}






