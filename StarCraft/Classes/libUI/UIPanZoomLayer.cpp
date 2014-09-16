
#include "UIPanZoomLayer.h"
#include "UIPlaceBuildingLayer.h"

UIPanZoomLayer::UIPanZoomLayer()
{
	_map = nullptr;
	_buildinglayer = nullptr;
}


UIPanZoomLayer::~UIPanZoomLayer()
{
     _delegate = NULL;
}


UIPanZoomLayer* UIPanZoomLayer::create()
{
	UIPanZoomLayer *pRet = new UIPanZoomLayer;
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}

	CC_SAFE_DELETE( pRet );
	return NULL;
}


bool UIPanZoomLayer::init()
{
	if( LayerColor::initWithColor( Color4B( 0, 0, 255, 0 ) ) == false )
		return false;
	
	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint( Point( 0, 0 ) );


	_accelerationFactor = 0.0f;
	_productFactor = 55.0f;

	_maxScale = 2.5f;
	_minScale = 1.0f;

	_isHolding = false;

	m_bIsNeedLongTouch = true;
	m_bIsLongTouched = false;
	m_bIsMaybeLongTouched = false;
	m_lTouchBeganTime = .0f;
    _delegate = NULL;

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UIPanZoomLayer::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UIPanZoomLayer::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UIPanZoomLayer::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void UIPanZoomLayer::onEnter()
{
	Layer::onEnter();
	Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);

}


void UIPanZoomLayer::onExit()
{
	Director::getInstance()->getScheduler()->unscheduleAllForTarget( this );
	Layer::onExit();

	_touches.clear();
	
}

void UIPanZoomLayer::update( float dt )
{   

	if (m_bIsNeedLongTouch && m_bIsMaybeLongTouched)
	{
		//大于2秒算长按
		if (_getMillisecondTime() - m_lTouchBeganTime >= 1.0f){

			m_bIsMaybeLongTouched = false;
			m_bIsLongTouched = true;

		}
	}
    
    
	// Skip smoothe panning when dt is high value
	if( dt > 1.0f / 55 )
		return;
    
    
    if (m_bIsNeedLongTouch && m_bIsLongTouched)
    {
        if (_delegate) {
            PointArray * array = _getPointArray();
            _delegate->doLongPress(array);
        }
        //Log("long press %f",_getMillisecondTime());
    }

    
    
    
	LayerColor::update( dt );

	if( _touches.size() == 1 )
	{
		_accelerationFactor *= 40 * dt * 0.95f;
	}
	else if( _touches.size() == 0 )
	{
		_accelerationFactor = fabs( _accelerationFactor - 0 );
		if( _accelerationFactor < FLT_EPSILON )
			return;

		if( _accelerationFactor < 0.004f )
		{
			_accelerationFactor = 0;
		}
		else
		{
			double d = dt * 60;
			if( d > 0.99 )
				d = 0.99;
			double i = (0 - _accelerationFactor) * 0.025 * d;

			_accelerationFactor = ( _accelerationFactor + i ) * d;

			Point adder = _deltaSum;
			adder.x *= this->getContentSize().width;
			adder.y *= this->getContentSize().height;

			this->setPosition( this->getPosition() + adder * 2.5 * _accelerationFactor );
		}
	}
}


void UIPanZoomLayer::setPosition( Point position )
{
	Node::setPosition( position );

	if( _panBoundsRect.equals( Rect() ) == false )
	{
		Rect boundBox;
		boundBox.origin = this->getPosition() / this->getScale();
		boundBox.size = this->getContentSize() / this->getScale();
		//Log( "boundBox : origin(%.1f, %.1f), size(%.1f, %.1f)", boundBox.origin.x, boundBox.origin.y, boundBox.size.width, boundBox.size.height );

		// OpenGL coordinate system
		float left = boundBox.getMinX();
		float right = boundBox.getMaxX();
		float top = boundBox.getMaxY();
		float bottom = boundBox.getMinY();
		//Log( "left,right(%.1f, %.1f), top,bottom(%.1f, %.1f)", left, right, top, bottom );

		float min_x = _panBoundsRect.getMinX() + boundBox.size.width;
		float max_x = _panBoundsRect.getMaxX() + boundBox.size.width;
		float min_y = _panBoundsRect.getMinY() + boundBox.size.height;
		float max_y = _panBoundsRect.getMaxY() + boundBox.size.height;
		//Log( "min(%.1f, %.1f), max(%.1f, %.1f)", min_x, min_y, max_x, max_y );

		float scale = this->getScale();
		float arLeft = min_x * scale;
		float arRight = max_x * scale - this->getContentSize().width;
		float arTop = max_y * scale - this->getContentSize().height;
		float arBottom = min_y * scale;
		
		if( left < min_x )
		{
			Node::setPosition( arLeft, this->getPosition().y );
		}

		if( right > max_x )
		{
			Node::setPosition( arRight, this->getPosition().y );
		}

		if( top > max_y )
		{
			Node::setPosition( this->getPosition().x, arTop );
		}

		if( bottom < min_y )
		{
			Node::setPosition( this->getPosition().x, arBottom );
		}
	}
}


void UIPanZoomLayer::setScale( float scale )
{
	LayerColor::setScale( MIN( MAX( scale, _minScale ), _maxScale ) );
	this->setPosition( this->getPosition() );
}


void UIPanZoomLayer::SetPanBoundsRect( Rect rect )
{
	_panBoundsRect = rect;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	float wFactor = _panBoundsRect.size.width / visibleSize.width;
	float hFactor = _panBoundsRect.size.height / visibleSize.height;
	
	//可以缩小的因素 不出现黑屏
	if (wFactor > 1)
	{  
		wFactor = 1 / wFactor;
	}
	if (hFactor > 1)
	{
		hFactor = 1 / hFactor;
	}
	

	float minScale;
	if( wFactor > hFactor )
		minScale = wFactor;
	else
		minScale = hFactor;
	SetMinScale( minScale );
}


void UIPanZoomLayer::SetMaxScale( float maxScale )
{
	_maxScale = maxScale;
}


float UIPanZoomLayer::GetMaxScale()
{
	return _maxScale;
}


void UIPanZoomLayer::SetMinScale( float minScale )
{
	_minScale = minScale;
}


float UIPanZoomLayer::GetMinScale()
{
	return _minScale;
}


void UIPanZoomLayer::Holding()
{
	_isHolding = true;
}


void UIPanZoomLayer::UnHolding()
{
	_isHolding = false;
}


void UIPanZoomLayer::SetProductFactor( float v )
{
	_productFactor = v;
}

 float UIPanZoomLayer::_getMillisecondTime()
 {
	  struct timeval now;
	 gettimeofday(&now, NULL); 
	 int temp = (int)(now.tv_sec * 1000 + now.tv_usec / 1000);
	 return temp/1000.f;
 }

void UIPanZoomLayer::SetDelegate(UILongPressOrClickDelegate * delegate)
{
    this->_delegate = delegate;
}

PointArray * UIPanZoomLayer:: _getPointArray()
{
    PointArray * points = PointArray::create(_touches.size());
	for (auto & touch : _touches)
	{    
		 Point pt = _map->convertTouchToNodeSpace(touch);
		 points->addControlPoint(pt);
	}
    return points;
}

void UIPanZoomLayer::onTouchesBegan( const std::vector<Touch*>& pTouches, Event *event )
{
	if( _isHolding ) return;

	for (auto &touch : pTouches )
	{
		_touches.pushBack(touch);
	}

	_deltaSum = Point( 0, 0 );
	_accelerationFactor = 0;
	gettimeofday( &_timeStamp, NULL );

	// 需要长按
	if (m_bIsNeedLongTouch)
	{
		PointArray * array = _getPointArray();
		if (_delegate && _delegate->checkIsCanClickOrLongPress(array))
		{
			m_bIsMaybeLongTouched = true;
			m_lTouchBeganTime = _getMillisecondTime();
		}

	}
}

void UIPanZoomLayer::onTouchesMoved( const std::vector<Touch*>& touches, Event *event )
{
	if( _isHolding ) return;
	if (_buildinglayer != nullptr && _buildinglayer->getIsMove()) return;

	if (m_bIsNeedLongTouch && !m_bIsLongTouched)
	{
		m_bIsMaybeLongTouched = false;
	}

	if (!m_bIsLongTouched && !m_bIsMaybeLongTouched)
	{   

		if( _touches.size() == 1 )
		{
			Touch *touch =  _touches.at(0);


			Point curTouchPosition = Director::getInstance()->convertToGL( touch->getLocationInView() );
			Point prevTouchPosition = Director::getInstance()->convertToGL( touch->getPreviousLocationInView() );
			Point deltaPosition = curTouchPosition - prevTouchPosition;
			this->setPosition( this->getPosition() + deltaPosition );

			float prevAngle = CC_RADIANS_TO_DEGREES( _prevDeltaPoint.getAngle() );
			float angle = CC_RADIANS_TO_DEGREES( deltaPosition.getAngle() );
			if( fabs( prevAngle - angle ) <= 30 )
			{
				_deltaSum = Point( 0, 0 );
			}

			_prevDeltaPoint = deltaPosition;

			_deltaSum.x = _deltaSum.x + deltaPosition.x / this->getContentSize().width;
			_deltaSum.y = _deltaSum.y + deltaPosition.y / this->getContentSize().height;

			_accelerationFactor += _deltaSum.getLength() * 4.0;
		}
		else if( _touches.size() >= 2 )
		{
			// Get the two first touches
			Touch *touch1 = _touches.at(0);
			Touch *touch2 = _touches.at(1);

			// Get current and previous positions of the touches
			Point curPosTouch1 = Director::getInstance()->convertToGL(touch1->getLocationInView());
			Point curPosTouch2 = Director::getInstance()->convertToGL(touch2->getLocationInView());
			Point prevPosTouch1 = Director::getInstance()->convertToGL(touch1->getPreviousLocationInView());
			// fix bug
			Point prevPosTouch2;

			Point tmptouch2 = touch2->getPreviousLocationInView();
			if (tmptouch2.equals(Point()) )
			{
				prevPosTouch2 = curPosTouch2;
			}
			else
			{
				prevPosTouch2 = Director::getInstance()->convertToGL(touch2->getPreviousLocationInView());
			}
			// Calculate current and previous positions of the layer relative the anchor point
		
			Point curPosLayer =  curPosTouch1.getMidpoint(curPosTouch2);
			Point prevPosLayer = prevPosTouch1.getMidpoint(prevPosTouch2);;

			// Calculate new scale
			float prevScale = this->getScale(); 
		
			float curScale = this->getScale() *  curPosTouch1.getDistance(curPosTouch2) / prevPosTouch1.getDistance(prevPosTouch2);

			if (fabsf(prevScale - curScale) > 0.01)
			{


				this->setScale( curScale );

				if( this->getScale() != prevScale )
				{
					Point realCurPosLayer = this->convertToNodeSpaceAR(curPosLayer);
					float deltaX = (realCurPosLayer.x) * (this->getScale() - prevScale);
					float deltaY = (realCurPosLayer.y) * (this->getScale() - prevScale);


					this->setPosition(Point(this->getPosition().x - deltaX, this->getPosition().y - deltaY));


					// If current and previous position of the multitouch's center aren't equal -> change position of the layer
					if (!prevPosLayer.equals(curPosLayer))
					{

						this->setPosition(Point(this->getPosition().x + curPosLayer.x - prevPosLayer.x,
							this->getPosition().y + curPosLayer.y - prevPosLayer.y));

					}

				}


			}


		}

	}

}

void UIPanZoomLayer::onTouchesEnded( const std::vector<Touch*>& pTouches, Event *event )
{
	if( _isHolding ) return;

	if ( m_bIsNeedLongTouch && m_bIsMaybeLongTouched)
	{
		if (_delegate) {
			PointArray * array = _getPointArray();
			_delegate->doClick(array);
		}
		//Log("click %f",_getMillisecondTime());
	}

	for (auto &touch : pTouches )
	{
		_touches.eraseObject(touch);
	}

	//TODO
	_touches.clear();
	
	m_bIsLongTouched = false;
	m_bIsMaybeLongTouched = false;
	m_lTouchBeganTime = 0;
}

void UIPanZoomLayer::setMap(Node * map,bool isInbattle)
{
	if (_map == nullptr  && _buildinglayer == nullptr)
	{
		_map = map;
		_buildinglayer = UIPlaceBuildingLayer::create();
		_buildinglayer->setContentSize(map->getContentSize());
		_buildinglayer->setPosition(Point::ZERO);
		this->SetPanBoundsRect( CCRect(
			map->getContentSize().width * -0.5,
			map->getContentSize().height * -0.5,
			map->getContentSize().width * 1.0,
			map->getContentSize().height * 1.0) );
	    _buildinglayer->setIsInBattle(isInbattle);	 
		this->addChild(_map);
		_map->addChild(_buildinglayer);
	}
}

void UIPanZoomLayer::addBuilding( Building * build,int zorder /*= 0*/ )
{
	if (_buildinglayer != nullptr)
	{
		_buildinglayer->AddBuilding(build,zorder);
	}
}

UIPlaceBuildingLayer * UIPanZoomLayer::getPlaceLayer()
{
	return _buildinglayer;
}

void UIPanZoomLayer::addNewBuilding( Building * build )
{
	if (_buildinglayer != nullptr)
	{   
		auto pt = _map->convertToNodeSpace(Point(Director::getInstance()->getWinSize().width / 2,Director::getInstance()->getWinSize().height / 2));
		auto rpt =  MapHelper::convertPointToCoord(pt);
		if (rpt.x % 2 != 0)
		{
			rpt.x -= 1;
		}
		if (rpt.y %2 != 0)
		{
			rpt.y -= 1;
		}	
		build->setCoor(Coord(rpt.x,rpt.y));
		_buildinglayer->AddNewBuilding(build);
	}
}










