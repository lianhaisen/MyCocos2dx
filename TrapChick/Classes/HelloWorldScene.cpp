#include "HelloWorldScene.h"
#include "time.h"
USING_NS_CC;
const int MAXROW = 9;
const int MAXCOLLUM = 9; 
const int SPACEH1 = 30;
const int SPACEH2 = 45;
const int SPACEV = 30;
const int CIRCLEWIDTH = 30;
const int CIRCLEHEIGHT = 30;



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    } 

	initCircle();

	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Code.plist");
	

	_chicken = Chicken::createFromLayer(this);
	_chicken->setAnchorPoint(Point(0.5,0));
	int posnum = MAXROW * MAXCOLLUM /2 ;
	_chicken->setCoor(Point(posnum / MAXROW,posnum % MAXCOLLUM));
	


	_moveCountLabel = Label::createWithBMFont("fonts/Arial.fnt","Moves: 0");
	_moveCountLabel->setPosition(250,400);
	this->addChild(_moveCountLabel);
	

	auto lister = EventListenerTouchOneByOne::create();
	lister->setSwallowTouches(true);
	lister->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lister, this);

    return true;
}

Point HelloWorld::getPositionFromCoor(Point coor)
{  
	int i = (int)coor.x;
	int j = (int)coor.y;
	auto pos = Point::ZERO;
	if (i % 2 == 0)
	{
		pos = Point(SPACEH1 + CIRCLEWIDTH * j,SPACEV + CIRCLEHEIGHT*i);
	}
	else
	{
		pos = Point(SPACEH2 + CIRCLEWIDTH * j,SPACEV + CIRCLEHEIGHT*i);
	}
	return pos;
}

void HelloWorld::initCircle()
{
	for (int i = 0 ; i < MAXROW ; i++)
	{
		for (int j = 0 ; j < MAXCOLLUM; j++)
		{    
			auto sp = CircleSprite::createFromCoorAndLayer(Point(i,j),this);
			sp->setPosition(getPositionFromCoor(Point(i,j)));
		}
	}

	long long tt;
    struct timeval tv;
    gettimeofday(&tv,NULL);
    tt = tv.tv_sec * 1000 + tv.tv_usec / 1000;
 	srand(tv.tv_usec);
	auto circlecount = 5 + rand() % 15;
	for (int i = 0;i<circlecount;i++)
	{   
		tt += 1000;
		srand(tt);
		auto value = rand() % (MAXCOLLUM * MAXROW);
		if (value != (MAXROW * MAXCOLLUM) / 2)
		{
			auto circle = _circles.at(value);
			circle->setSelect();
		}
		
	}
	

}

void HelloWorld::addCircle( CircleSprite * circle )
{
	_circles[circle->getId()] = circle;
	this->addChild(circle);
}

bool HelloWorld::onTouchBegan( Touch *touch, Event *unused_event )
{   
	if(_gameOver)
	{
		return false;
	}
	if (!_circles.empty())
	{
		for (auto pair : _circles)
		{
			auto circle = pair.second;
			Point local = convertToNodeSpace(touch->getLocation());
			Rect r = circle->getBoundingBox();
			if (r.containsPoint(local))
			{   
				if (!circle->IsSelect())
				{   
				    circle->setSelect();
					addMove();
				}
				break;
			}
		}
	}
	return true;
}

HelloWorld::HelloWorld()
{
    _gameOver = false;
	_won = false;
	_moveCount = 0;
	_moveCountLabel = nullptr;
	_left = 0;
	_right = 0;
	_up = 0;
	_down = 0;
}

CircleSprite * HelloWorld::getSpriteFromCoor(Point coor)
{   
    int i = (int)coor.x;
	int j = (int)coor.y;
	return _circles.at(i*MAXROW + j);
}

void HelloWorld::showMoves( int num )
{
	_moveCountLabel->setString(String::createWithFormat("Moves:%d",num)->getCString());
}

void HelloWorld::addMove()
{
	_moveCount += 1;
	showMoves(_moveCount);

	if (checkIsLoseFromCoor(_chicken->getCoor()))
	{   
		_won = false;
		_gameOver = true;
		_chicken->setVisible(false);
		showRestartMenu();
		return;
	}

	auto steps = getValidStep(_chicken->getCoor());
	if (steps->count() <= 0)
	{
		_won = true;
		_gameOver = true;
		showRestartMenu();
		return;
	}

	if (!_chicken->IsTrap())
	{
		int stepCount = INT32_MAX;
		Point step = Point::ZERO;
		Point currentstep = _chicken->getCoor();
		for (int i = 0; i < 6; i++)
		{   
			Point target = getTarget(currentstep,(MoveType)i);	
			auto computepath = ComputePath::createFromLayer(this);
			computepath->getPath(currentstep,target);
			int tmpstepcount = computepath->getStepCount();
			if (tmpstepcount != 0 && tmpstepcount < stepCount)
			{  
				stepCount = tmpstepcount;
				step = computepath->getFirstStep();
			}
		}

		if (stepCount == INT32_MAX)
		{
			_chicken->setTrap();
		}
		else
		{   
			_chicken->setCoor(step);
			return;
		}
	}
	
	if (_chicken->IsTrap())
	{   

		auto currentSelect = Point::ZERO;
		auto cost = INT32_MAX;
		for (int i = 0; i < steps->count(); i++)
		{
			auto pt = steps->getControlPointAtIndex(i);
			auto tmpcost = abs(0 - pt.x) + _down;
			tmpcost = MIN(tmpcost,abs(MAXROW - 1 - pt.x + _up) ) ;
			tmpcost = MIN(tmpcost,abs(0 - pt.y) + _left);
			tmpcost = MIN(tmpcost,abs(MAXCOLLUM - 1 - pt.x + _right));

			if (tmpcost < cost)
			{   
				cost = tmpcost;			
				currentSelect = pt;
			}
		}
		_chicken->setCoor(currentSelect);
	}
	





	

}

bool HelloWorld::checkIsLoseFromCoor( Point coor )
{
	auto x = (int)coor.x;
	auto y = (int)coor.y;
	if (x == 0 || x == MAXROW-1 || y == 0 || y == MAXCOLLUM-1 )
	{
		return true;
	}
	return false;
}

void HelloWorld::showRestartMenu()
{
	auto winSize = Director::getInstance()->getWinSize();
	std::string message;
	if (_won)
	{
		message = "You Win!";
	}
	else
	{
		message = "You lose!";
	}
	auto label = Label::createWithBMFont("fonts/Arial.fnt",message);
	label->setScale(0.1f);
	label->setPosition(winSize.width/2,winSize.height*0.6);
	this->addChild(label);

	auto restartLabel = Label::createWithBMFont("fonts/Arial.fnt","Restart");
	auto restartitem = MenuItemLabel::create(restartLabel,CC_CALLBACK_1(HelloWorld::restartTapped,this));
	restartitem->setScale(0.1f);
	restartitem->setPosition(winSize.width / 2,winSize.height * 0.4);

	auto menu = Menu::createWithItem(restartitem);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,10);

	restartitem->runAction(ScaleTo::create(0.5f,1.0f));
	label->runAction(ScaleTo::create(0.5f,1.0f));  
}



void HelloWorld::restartTapped( Ref * ref )
{
	auto scene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(scene);
}

PointArray * HelloWorld::getValidStep( Point coor )
{   
	auto arrary = PointArray::create(6);
	int x = coor.x;
	int y = coor.y;
	
	//left
	if (y - 1 >=0)
	{
		auto vcoor = Point(x,y - 1);
		if (!getSpriteFromCoor(vcoor)->IsSelect())
		{
			arrary->addControlPoint(vcoor);
		}
	}
	//right
	if (y + 1 < MAXCOLLUM)
	{
		auto vcoor = Point(x,y + 1);
		if (!getSpriteFromCoor(vcoor)->IsSelect())
		{
			arrary->addControlPoint(vcoor);
		}
	}
	if (x % 2 == 0)
	{
		//topleft
		if (y - 1 >= 0 && x + 1 < MAXROW)
		{
			auto vcoor = Point(x+1,y-1);
			if (!getSpriteFromCoor(vcoor)->IsSelect())
			{
				arrary->addControlPoint(vcoor);
			}
		}
		//top right
		if (y < MAXCOLLUM && x + 1 < MAXROW)
		{
			auto vcoor = Point(x+1,y);
			if (!getSpriteFromCoor(vcoor)->IsSelect())
			{
				arrary->addControlPoint(vcoor);
			}
		}
		
		//bottom  left

		if (y - 1 >= 0 && x - 1 >= 0)
		{
			auto vcoor = Point(x-1,y-1);
			if (!getSpriteFromCoor(vcoor)->IsSelect())
			{
				arrary->addControlPoint(vcoor);
			}
		}
		
		//bottom right
		if (y  < MAXCOLLUM && x - 1 >= 0)
		{
			auto vcoor = Point(x-1,y);
			if (!getSpriteFromCoor(vcoor)->IsSelect())
			{
				arrary->addControlPoint(vcoor);
			}
		}
	}
	else
	{
		//topleft
		if (y  >= 0 && x + 1 < MAXROW)
		{
			auto vcoor = Point(x+1,y);
			if (!getSpriteFromCoor(vcoor)->IsSelect())
			{
				arrary->addControlPoint(vcoor);
			}
		}
		//top right
		if (y + 1< MAXCOLLUM && x + 1 < MAXROW)
		{
			auto vcoor = Point(x+1,y + 1);
			if (!getSpriteFromCoor(vcoor)->IsSelect())
			{
				arrary->addControlPoint(vcoor);
			}
		}

		//bottom  left

		if (y  >= 0 && x - 1 >= 0)
		{
			auto vcoor = Point(x-1,y);
			if (!getSpriteFromCoor(vcoor)->IsSelect())
			{
				arrary->addControlPoint(vcoor);
			}
		}

		//bottom right
		if (y + 1 < MAXCOLLUM && x - 1 >= 0)
		{
			auto vcoor = Point(x-1,y+1);
			if (!getSpriteFromCoor(vcoor)->IsSelect())
			{
				arrary->addControlPoint(vcoor);
			}
		}
	}
	return arrary;

}

void HelloWorld::addCost( Point coor )
{
	_left  += (MAXCOLLUM - abs(coor.y - 0)) ;
	_right += ( MAXCOLLUM - abs(MAXCOLLUM - 1 - coor.y )) ;
	_up    += (MAXROW - abs(MAXROW - 1 - coor.x));
	_down  += ( MAXROW - abs(coor.x - 0)) ;
}

Point HelloWorld::getTarget( Point coor, MoveType movetype )
{   
	Point resutl = coor;
	switch (movetype)
	{
	case HelloWorld::MoveType::LEFT:
		resutl = Point(coor.x,0);
		break;
	case HelloWorld::MoveType::RIGTH:
		resutl = Point(coor.x,MAXCOLLUM - 1);
		break;
	case HelloWorld::MoveType::TOPLEFT:
		{
			for (int i = 0 ; i < MAXROW - 1 - coor.x; i++)
			{   
				if (int(resutl.x) % 2 != 0)
				{
					resutl.y -= 1;
				}	
				resutl.x += 1;
				if (resutl.y == 0 )
				{
				   break;
				}	
			}
		}
		break;
	case HelloWorld::MoveType::TOPRIGHT:
		{
			for (int i = 0 ; i < MAXROW - 1 - coor.x; i++)
			{   
				if (int(resutl.x) % 2 == 0)
				{
					resutl.y += 1;
				}	
				resutl.x += 1;
				if (resutl.y == MAXCOLLUM - 1 )
				{
					break;
				}	
			}
		}
		break;
	case HelloWorld::MoveType::BOTTOMLEFT:
		{
			for (int i = 0 ; i <coor.x - 0; i++)
			{   
				if (int(resutl.x) % 2 != 0)
				{
					resutl.y -= 1;
				}	
				resutl.x -= 1;
				if (resutl.y == 0 )
				{
					break;
				}	
			}
		}
		break;
	case HelloWorld::MoveType::BOTTOMRIGHT:
		{
			for (int i = 0 ; i <coor.x - 0; i++)
			{   
				if (int(resutl.x) % 2 == 0)
				{
					resutl.y += 1;
				}	
				resutl.x -= 1;
				if (resutl.y == MAXCOLLUM - 1 )
				{
					break;
				}	
			}
		}
		break;
	default:
		break;
	}
	return resutl;
}



CircleSprite::CircleSprite()
{  
	_layer = nullptr;
	_coor = Point::ZERO;
	_isselect = false;
}

bool CircleSprite::initWithCoorAndLayer( Point cooor,HelloWorld * layer )
{
	_layer = layer;
	_coor = cooor;
    this->initWithFile("normal.png");
	layer->addCircle(this);
	return true;
}

int CircleSprite::getId()
{
	return int(_coor.x * MAXROW + _coor.y);
}

void CircleSprite::setSelect()
{
	if (!_isselect)
	{    
		if (_layer)
		{  
			_layer->addCost(_coor);
		}
		
		this->setTexture("select.png");
		_isselect  = true;
	}
	
}

CircleSprite * CircleSprite::createFromCoorAndLayer( Point cooor,HelloWorld * layer )
{
	auto item = new CircleSprite();
	if (item->initWithCoorAndLayer(cooor,layer))
	{
		item->autorelease();
		return item;
	}
	return nullptr;
}

Chicken::Chicken()
{
	_layer = nullptr;
	_currentAnimation = nullptr;
	_normalAnimation = nullptr;
	_trapAnimation = nullptr;
	_curAnimate = nullptr;
	_isTrap = false;
}
Chicken::~Chicken()
{
    CC_SAFE_RELEASE(_normalAnimation);
	CC_SAFE_RELEASE(_trapAnimation);
}


bool Chicken::initWithLayer( HelloWorld * layer )
{
	if (initWithSpriteFrameName("chicken_001.png"))
	{   
		_layer = layer;
		auto animation = Animation::create();
		for (int i = 1; i <= 40; i++)
		{   
			auto str = String::createWithFormat("chicken_%03d.png",i);
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str->getCString());
			animation->addSpriteFrame(frame);
		}
		animation->setDelayPerUnit(1.0f / 1000 * 90);
		setNormalAnimation(animation);

		animation = Animation::create();
		for (int i = 1; i <= 12; i++)
		{   
			auto str = String::createWithFormat("ccchick_%03d.png",i);
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str->getCString());
			animation->addSpriteFrame(frame);
		}
		animation->setDelayPerUnit(1.0f / 1000 * 40);
		setTrapAnimation(animation);
		layer->addChild(this);

		runAnimation(getNormalAnimation());

		return true;
	}
	return false;
}

void Chicken::runAnimation( Animation * animaton )
{
	if (_currentAnimation == animaton)
	{
		return;
	}
	_currentAnimation = animaton;
	if (_curAnimate != nullptr)
	{
		this->stopAction(_curAnimate);
	}
	_curAnimate = RepeatForever::create(Animate::create(animaton));
	this->runAction(_curAnimate);

}

Chicken * Chicken::createFromLayer( HelloWorld * layer )
{
	auto item = new Chicken();
	if (item->initWithLayer(layer))
	{
		item->autorelease();
		return item;
	}
	return nullptr;

}

void Chicken::setCoor(Point coor)
{
	_coor = coor;
	if (_layer)
	{   
		setPosition(_layer->getPositionFromCoor(coor));
	}
	
}

void Chicken::setTrap()
{
	if (!_isTrap)
	{   
		_isTrap = true;
		runAnimation(getTrapAnimation());
	}
}



bool ShortestPathStep::initWithPositon( Point pos )
{
	position = pos;
	gScore = 0;
	hScore = 0;
	parent = nullptr;
	return true;

}

bool ShortestPathStep::isEqual( ShortestPathStep * other )
{
	return this->getPos()  == other->getPos();
}

int ShortestPathStep::getFScore()
{
	return this->getGScore() + this->getHScore();
}

std::string ShortestPathStep::getInfo()
{
	auto str = String::createWithFormat("pos=[%f;%f] g=%d h=%d f=%d",this->getPos().x,this->getPos().y,this->getGScore(),this->getHScore(),this->getFScore());
	return str->getCString();
}

ShortestPathStep * ShortestPathStep::createFromPos( Point pos )
{
	auto item = new ShortestPathStep();
	if (item->initWithPositon(pos))
	{
		item->autorelease();
		return item;
	}
	return nullptr;
}

ComputePath * ComputePath::createFromLayer( HelloWorld * layer )
{
	 auto item = new ComputePath();
	 if ( item->initWithLayer(layer))
	 {
		 item->autorelease();
		 return item;
	 }
	 return nullptr;
}

bool ComputePath::initWithLayer( HelloWorld * layer )
{   
	_layer = layer;
	return true;
}

void ComputePath::insertInOpenSteps( ShortestPathStep* step )
{
	auto stepFScore = step->getFScore();
	auto count = spOpenSteps.size();
	int i = 0;
	for (;i < count ; i++)
	{
		if (stepFScore <=  spOpenSteps.at(i)->getFScore() )
		{   
			break;
		}
	}
	spOpenSteps.insert(i,step);
}

int ComputePath::computeHScoreFromCoord( Point from,Point to )
{
	return abs(to.x - from.x) + abs(to.y - from.y);
}

int ComputePath::costToMoveFromStep( ShortestPathStep* fromStep,ShortestPathStep* toStep )
{
	return 10;
}

void ComputePath::getPath( Point from,Point to )
{
	insertInOpenSteps(ShortestPathStep::createFromPos(from));
    do 
    {
		auto currentStep = spOpenSteps.at(0);
		spClosedSteps.pushBack(currentStep);
		spOpenSteps.erase(0);
		if (currentStep->getPos() == to)
		{
			constructPath(currentStep);
			spOpenSteps.clear();
			spClosedSteps.clear();
			break;
		}
		auto points = _layer->getValidStep(currentStep->getPos());				
		for (int i = 0 ; i < points->count(); i++)
		{
			auto v = points->getControlPointAtIndex(i);
			auto step = ShortestPathStep::createFromPos(v);
			int closeindex = -1;
			for (int i = 0 ; i < spClosedSteps.size();i++ )
			{
				if (spClosedSteps.at(i)->isEqual(step))
				{
					closeindex = i;
					break;
				}
			}
			if (closeindex != -1)
			{
				continue;
			}
			auto moveCost = this->costToMoveFromStep(currentStep,step);
			int index = -1;
			for (int i = 0; i < spOpenSteps.size();i++)
			{
				auto item = spOpenSteps.at(i);
				if (step->isEqual(item))
				{
					index = i;
					break;
				}
			}
			if (index == -1)
			{
				step->setParent(currentStep);
				step->setGScore(currentStep->getGScore() + moveCost);
				step->setHScore(this->computeHScoreFromCoord(step->getPos(),to));
				this->insertInOpenSteps(step);
			}
			else
			{   
				step = this->spOpenSteps.at(index);
				if (currentStep->getGScore() + moveCost < step->getGScore())
				{
					step->setGScore(currentStep->getGScore() + moveCost);
					step->retain();
					spOpenSteps.erase(index);
					this->insertInOpenSteps(step);
					step->release();
				}

			}

              
		}

    } while (spOpenSteps.size() > 0);
    
}

ComputePath::~ComputePath()
{
	spOpenSteps.clear();
	spClosedSteps.clear();
	shortestPath.clear();

	

}

void ComputePath::constructPath(ShortestPathStep* step)
{
	shortestPath.clear();
	do 
	{   
		if (step->getParent() != nullptr)
		{
			shortestPath.insert(0,step);
		}
		step = step->getParent();
	} while (step != nullptr);


}
