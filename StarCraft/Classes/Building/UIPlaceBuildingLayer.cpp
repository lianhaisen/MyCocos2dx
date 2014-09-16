#include "UIPlaceBuildingLayer.h"
#include "MapHelper.h"
#include "AppMacros.h"
#include "BuildingData.h"
#include "BuildingConfig.h"
#include "BuildingCommon.h"

bool UIPlaceBuildingLayer::init()
{    
	ignoreAnchorPointForPosition(true); 


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(UIPlaceBuildingLayer::onTouchesBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(UIPlaceBuildingLayer::onTouchesMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(UIPlaceBuildingLayer::onTouchesEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(UIPlaceBuildingLayer::onTouchCancelled,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_lister = listener;
	_lister->setSwallowTouches(false);
	return true;
}

UIPlaceBuildingLayer::UIPlaceBuildingLayer()
{
	_lister = nullptr;
	_selectItem = nullptr;
	_chooseItem = nullptr;
	_isMoving = false;
	_isMoveScreen = false;
	_LayerStatus = BLayerStatus::NOMAL;
	_buyBuliding = nullptr;
}

UIPlaceBuildingLayer::~UIPlaceBuildingLayer()
{
	 for (auto pair : _walls)
	 {   
		 auto wallcoor = pair.second;
		 if (wallcoor)
		 {   
			 delete wallcoor;
		 }
	 }
	_walls.clear();
}

void UIPlaceBuildingLayer::AddBuilding( Building * building,int zorder )
{
	_buildings.push_back(building);
	 this->addChild(building,zorder);
	 addWalls(building);
	 building->addBuildPoint();
}


void UIPlaceBuildingLayer::AddNewBuilding( Building * building )
{    
	//选择的去选择
	if (_selectItem)
	{
		_selectItem->unselected();
		_selectItem = nullptr;
	}
	//之前买的,去掉
	if (_buyBuliding)
	{
		_buyBuliding->removeFromParent();
		_buyBuliding = nullptr;
	}

	_LayerStatus = BLayerStatus::BUY;
    _buyBuliding = building;
	
	this->addChild(_buyBuliding,(MAX_GRID_COUNT + 1 )* (MAX_GRID_COUNT + 1));
	addWalls(building);

	//设置购买模式
	_buyBuliding->setBuy(true,CC_CALLBACK_2(UIPlaceBuildingLayer::onBuyYesOrNo,this));
	if (!_buyBuliding->checkaddPoint())
	{
		_buyBuliding->setillegal(true);
	}
	

	
}


bool UIPlaceBuildingLayer::onTouchesBegan( Touch * touch, cocos2d::Event *event )
{    
	
	//普通模式
	if (_LayerStatus == BLayerStatus::NOMAL)
	{
		_chooseItem = getItemForTouch(touch);	
		if(_chooseItem == _selectItem && _selectItem != nullptr)
		{   
			_touchBeginPoint = convertTouchToNodeSpace(touch);
			_touchDiff = _touchBeginPoint - _chooseItem->getPosition(); 
		}
	}
	//购买模式
	else
	{
		if (isInTouch(_buyBuliding,touch))
		{
			_chooseItem = _buyBuliding;
			_touchBeginPoint = convertTouchToNodeSpace(touch);
			_touchDiff = _touchBeginPoint - _chooseItem->getPosition(); 
		}
	}
	

	return true;
	
}

void UIPlaceBuildingLayer::onTouchesMoved( Touch * touch, cocos2d::Event *event )
{   
	//普通模式
	if (_LayerStatus == BLayerStatus::NOMAL)
	{
		if(_chooseItem == _selectItem && _selectItem != nullptr)
		{  
			if (!_isMoving )
			{  
				_chooseItem->removeBuildPoint();
			}
			_isMoving   = true;
			_selectItem->setMove(true);
			Point point0 = _selectItem->getPosition();
			Point point1 = this->convertTouchToNodeSpace(touch);


			Point diff = _touchBeginPoint - point1;
			int tempaddx =0,tempaddy = 0;
			tempaddx = diff.x / (UNIT_WIDTH*2);
			tempaddy = diff.y / (UNIT_HEIGHT*2);


			if (tempaddx != 0 || tempaddy != 0)
			{   
				//TODO 检查是否合法 
				//1.边界
				//2.其他建筑物这段
				//_selectItem->setillegal(true);
				auto cx  = _chooseItem->getCoor().x;
				auto cy  = _chooseItem->getCoor().y;
				cx += tempaddx * 2;
				cy += tempaddy * 2;
				_chooseItem->setCoor(Coord(cx,cy));
				if (!_chooseItem->checkaddPoint())
				{
					_chooseItem->setillegal(true);
				}
				else
				{
					_chooseItem->setillegal(false);
				}
				_touchBeginPoint = MapHelper::getPointFromCoord(cx,cy) + _touchDiff;
			}
		}
		else
		{
			_isMoveScreen = true;
		}
	}
	//购买模式
	else
	{
		if(_chooseItem)
		{
			_isMoving   = true;
			_chooseItem->setMove(true);
			Point point0 = _chooseItem->getPosition();
			Point point1 = this->convertTouchToNodeSpace(touch);

			Point diff = _touchBeginPoint - point1;
			int tempaddx =0,tempaddy = 0;
			tempaddx = diff.x / (UNIT_WIDTH*2);
			tempaddy = diff.y / (UNIT_HEIGHT*2);

			if (tempaddx != 0 || tempaddy != 0)
			{   
				//TODO 检查是否合法 
				//1.边界
				//2.其他建筑物这段
				//_selectItem->setillegal(true);
				auto cx  = _chooseItem->getCoor().x;
				auto cy  = _chooseItem->getCoor().y;
				cx += tempaddx * 2;
				cy += tempaddy * 2;
				_chooseItem->setCoor(Coord(cx,cy));
				if (!_chooseItem->checkaddPoint())
				{
					_chooseItem->setillegal(true);
				}
				else
				{
					_chooseItem->setillegal(false);
				}
				_touchBeginPoint = MapHelper::getPointFromCoord(cx,cy) + _touchDiff;
			}
		}
		else
		{
			_isMoveScreen = true;
		}
	}



}

void UIPlaceBuildingLayer::onTouchesEnded( Touch * touch, cocos2d::Event *event )
{  

	if (_LayerStatus == BLayerStatus::NOMAL)
	{
		if (_isMoving && _selectItem != nullptr)
		{
			_selectItem->setMove(false);
		}

		if (!_isMoving && !_isMoveScreen)
		{
			if (_chooseItem == getItemForTouch(touch) && _chooseItem != nullptr)
			{   
				if (_chooseItem == _selectItem && _selectItem!=nullptr)
				{  
					_selectItem->unselected();
					_selectItem = nullptr;
				}
				else
				{
					if (_selectItem != nullptr)
					{
						_selectItem->unselected();
					}
					_selectItem = _chooseItem;
					_selectItem->selected();
				}

			}
			else
			{
				if (getItemForTouch(touch) == nullptr)
				{
					if (_selectItem != nullptr)
					{
						_selectItem->unselected();
					}
				}

			}
		}
	}
	else
	{
		if (_chooseItem)
		{
			_chooseItem->setMove(false);
		}
	}
   

	
	_chooseItem = nullptr;
	_isMoving   = false;
	_isMoveScreen = false;

}

void UIPlaceBuildingLayer::onTouchCancelled( Touch * touch, cocos2d::Event *event )
{
	log("cancle");
}

void UIPlaceBuildingLayer::draw( Renderer *renderer, const kmMat4 &transform, bool transformUpdated )
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(UIPlaceBuildingLayer::onDraw, this, transform, transformUpdated);
	renderer->addCommand(&_customCommand);


}

void UIPlaceBuildingLayer::onDraw( const kmMat4 &transform, bool transformUpdated )
{
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);

	DrawPrimitives::setDrawColor4B(0, 0, 0, 255);
	DrawPrimitives::drawLine(BuildingCommon::getTmpPoint(),BuildingCommon::getTmpPoint2());

	if (_isMoving)
	{
		Size size = getContentSize();
		Point p1 = POINT1;
		Point p2 = POINT2;
		Point p3 = POINT3;
		Point p4 = POINT4;
		Point points[4] = {p1, p2, p3, p4};
		DrawPrimitives::drawSolidPoly(points, 4, Color4F(0,1,0,0.5));

		float w = (p1.x-p2.x)/(MAX_GRID_COUNT/2);
		float h = (p1.y-p2.y)/(MAX_GRID_COUNT/2);

		DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
		for (int i = 0; i <= MAX_GRID_COUNT/2; i++)
		{
			DrawPrimitives::drawLine(Point(p1.x + w*i, p1.y - h*i), Point(p2.x + w*i, p2.y - h*i));
		}
		for (int i = 0; i <= MAX_GRID_COUNT/2; i++)
		{
			DrawPrimitives::drawLine(Point(p1.x - w*i, p1.y - h*i), Point(p4.x - w*i, p4.y - h*i));
		}
	}
	

	kmGLPopMatrix();
}

Building* UIPlaceBuildingLayer::getItemForTouch( Touch * touch )
{   
	
	Building* ritem = nullptr;
	if (!_buildings.empty() && touch)
	{   
		for (auto item : _buildings)
		{   
			if (item && item->isVisible())
			{                 
				Point local = convertToNodeSpaceAR(touch->getLocation());
				if (item->isPointInRect(local))
				{
					ritem = item;
					break;
				}
			}
		}
	}
	return ritem;
}

bool UIPlaceBuildingLayer::isInTouch( Building * item,Touch * touch )
{
	auto result = false;
	if (item)
	{
	    Point local = convertToNodeSpaceAR(touch->getLocation());
		Rect r = item->getBoundingBox();
		if (r.containsPoint(local))
		{
			result = true;
		}
	}
	return result;
}



void UIPlaceBuildingLayer::addbuildPoint( int x,int y,int gridcount )
{   
	gridcount *= 2;
	for (int i = x - gridcount ; i < x; i++)
	{
		for (int j = y - gridcount ; j < y ; j++)
		{
			_pointSet.insert(i * MAX_GRID_COUNT + j);
		}
	}
}

void UIPlaceBuildingLayer::removebuildingPoint( int x,int y,int gridcount )
{  
	gridcount *= 2;
	for (int i = x - gridcount; i < x; i++)
	{
		for (int j = y - gridcount; j < y ; j++)
		{
			_pointSet.erase(i * MAX_GRID_COUNT + j);
		}
	}
}

bool UIPlaceBuildingLayer::checkaddPoint( int x,int y,int gridcount )
{   
	gridcount *= 2;
	if (!MapHelper::checkIsInPlace(x,y,gridcount))
	{
		return false;
	}
	set<int> tmepset;
	std::copy(_pointSet.begin(),_pointSet.end(), insert_iterator<set<int> >(tmepset, tmepset.begin()));
	auto totalcount = _pointSet.size();
	for (int i = x - gridcount; i < x; i++)
	{
		for (int j = y - gridcount; j < y ; j++)
		{    
			tmepset.insert(i * MAX_GRID_COUNT + j);
		}
	}
	if ( totalcount + gridcount * gridcount != tmepset.size())
	{
		return false;
	}
	return true;
}

void UIPlaceBuildingLayer::setIsInBattle( bool isInbattle )
{
	if (isInbattle)
	{
		_eventDispatcher->removeEventListenersForTarget(this);
	}
}

void UIPlaceBuildingLayer::onBuyYesOrNo( Ref* ref,bool isyes )
{   

	if (isyes)
	{
		_buildings.push_back(_buyBuliding);
		_buyBuliding->addBuildPoint();
		_buyBuliding->setLocalZOrder(_buyBuliding->getCoor().y * MAX_GRID_COUNT + _buyBuliding->getCoor().x);
		_buyBuliding->setBuy(false);
		// TODO 绑定数据
		auto cfg = _buyBuliding->getBuyCfg();
		//cfg->current++;
		auto data = BuildingData::CreateDBuilding(cfg->buildcfg->id,_buyBuliding->getCoor().x,_buyBuliding->getCoor().y);
		_buyBuliding->setBuidingData(data);

	}
	else
	{    
	    auto type = _buyBuliding->getBType();
		if (type == 720001)
		{
			auto wall = dynamic_cast<UIWall*>(_buyBuliding);
			wall->releaseWallCoord();
		}
		_buyBuliding->removeFromParent();
	}

	_LayerStatus = BLayerStatus::NOMAL;
	_buyBuliding = nullptr;
}

void UIPlaceBuildingLayer::addWalls( Building * building )
{   
	auto type = building->getBType();
	if (type == 720001 )
	{    
		auto wall = dynamic_cast<UIWall*>(building);
        wall->setWallCoord(wall->getCoor());
	}
}

WallCoord * UIPlaceBuildingLayer::getWallCoord( Coord coord )
{
	WallCoord * item = nullptr;
	int Key = coord.x * 1000000 + coord.y;
	if (_walls.empty())
	{
		item = new WallCoord();
		item->key = Key;
		_walls[item->key] = item;
	}
	else
	{
		auto result = _walls.find(Key);
		if (result != _walls.end())
		{
			item = result->second;
		}
		else
		{
			item = new WallCoord();
			item->key = Key;
			_walls[item->key] = item;
		}
	}
	return item;
}

UIWall * UIPlaceBuildingLayer::getWallFromCoord( Coord coord )
{   
	UIWall * item = nullptr;
	int Key = coord.x * 1000000 + coord.y;
	if (!_walls.empty())
	{
		auto result = _walls.find(Key);
		if (result != _walls.end())
		{   
			auto wallcoord = result->second;
			auto list = wallcoord->walllist;
			for (auto sitem : list)
			{
				 if (sitem->getParent() != nullptr)
				 {   
					 item = sitem;
					 break;
				 }
			}
		}
	}

	return item;
}





