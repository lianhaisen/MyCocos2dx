#include "HelloWorldScene.h"
#include "CatSprite.h"
USING_NS_CC;
using namespace CocosDenshion;

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


bool HelloWorld::isValidTileCoord( Point tileCoord )
{
	if (tileCoord.x < 0 || tileCoord.y < 0 || tileCoord.x >= _tileMap->getMapSize().width || tileCoord.y >= _tileMap->getMapSize().height)
	{
		return false;
	}
	return true;
	
}

Point HelloWorld::tileCoordForPosition( Point position )
{
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y ) / _tileMap->getTileSize().height;
	return Point(x,y);
}

Point HelloWorld::positionForTileCoord( Point tileCoord )
{   
	int x = (tileCoord.x * _tileMap->getTileSize().width ) + _tileMap->getTileSize().width / 2;
	int y = (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - (tileCoord.y * _tileMap->getTileSize().height) - _tileMap->getTileSize().height / 2;
	return Point(x,y);
}

bool HelloWorld::isProp( std::string prop,Point tileCoord,TMXLayer * layer )
{
	if (!isValidTileCoord(tileCoord))
	{
		return false;
	}
	int gid = layer->getTileGIDAt(tileCoord);
	
	Value * tmpvalue;
	if(_tileMap->getPropertiesForGID(gid,&tmpvalue))
	{
		auto map = (*tmpvalue).asValueMap();
		return map.find(prop) != map.end();
	}
	return false;	
}

bool HelloWorld::isWallAtTileCoord( Point tileCoord )
{   
	return isProp("Wall",tileCoord,_bgLayer);
}

bool HelloWorld::isBoneAtTilecoord( Point tileCoord )
{
	return isProp("Bone",tileCoord,_objectLayer);
}

bool HelloWorld::isDogAtTilecoord( Point tileCoord )
{
	return isProp("Dog",tileCoord,_objectLayer);
}

bool HelloWorld::isExitAtTilecoord( Point tileCoord )
{
	return isProp("Exit",tileCoord,_objectLayer);
}

void HelloWorld::removeObjectAtTileCoord( Point tileCoord )
{
	_objectLayer->removeTileAt(tileCoord);
}

void HelloWorld::setViewPointCenter( Point pos )
{            

	auto winSize =  Director::getInstance()->getWinSize();
	auto x = MAX(pos.x,winSize.width / 2 ) ;
	auto y = MAX(pos.y,winSize.height / 2);
	x =  MIN(x,(_tileMap->getMapSize().width * _tileMap->getTileSize().width) - winSize.width / 2);
	y =  MIN(y,(_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	auto actualPosition = Point(x,y);
	auto centerOfView = Point(winSize.width / 2,winSize.height / 2);
	auto viewPoint  = centerOfView - actualPosition;
	_tileMap->setPosition(viewPoint);
}

void HelloWorld::restartTapped(Ref * ref)
{
	auto scene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(scene);
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

void HelloWorld::endScene()
{
	_cat->runAction(Sequence::create(
		                            ScaleBy::create(0.5,3.0),
									DelayTime::create(1.0f),
									ScaleTo::create(0.5,0),
									CallFunc::create(CC_CALLBACK_0( HelloWorld::showRestartMenu,this)),
									nullptr
		                                            ));
	_cat->runAction(RepeatForever::create(RotateBy::create(0.5,360)));
}

void HelloWorld::winGame()
{
	_gameOver = true;
	_won = true;
	SimpleAudioEngine::getInstance()->playEffect("Sounds/win.wav");
	endScene();

}

void HelloWorld::loseGame()
{
	_gameOver = true;
	_won = false;
	SimpleAudioEngine::getInstance()->playEffect("Sounds/lose.wav");
	endScene();
}

void HelloWorld::showNumBones( int num )
{
	_bonesCount->setString(String::createWithFormat("Bones: %d",num)->getCString());
}

cocos2d::PointArray*  HelloWorld::walkableAdjacentTilesCoordForTileCoord( Point tileCoord )
{
	auto tmp  = PointArray::create(8);
	bool t = false;
	bool l = false;
	bool b = false;
	bool r = false;

	//Top
	auto p = Point(tileCoord.x,tileCoord.y - 1);
	if (isValidTileCoord(p) && !isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
		t = true;
	}

	//Left
	p = Point(tileCoord.x - 1,tileCoord.y);
	if (isValidTileCoord(p) && !isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
		l = true;
	}

	//Bottom
	p = Point(tileCoord.x,tileCoord.y + 1);
	if (isValidTileCoord(p) && !isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
		b = true;
	}
	
	//Right
	p = Point(tileCoord.x + 1,tileCoord.y);
	if (isValidTileCoord(p) && !isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
		r = true;
	}
	
	//TOP LEFT
	p = Point(tileCoord.x - 1,tileCoord.y - 1);
	if (t && l && isValidTileCoord(p) && !isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
	}
	
	//BOTTOM LEFT
	p = Point(tileCoord.x - 1,tileCoord.y + 1);
	if (b && l &&  isValidTileCoord(p) && !isWallAtTileCoord(p) )
	{
		tmp->addControlPoint(p);
	}
	
	//TOP RIGHT
	p = Point(tileCoord.x + 1,tileCoord.y - 1);
	if (t && r && isValidTileCoord(p) && !isWallAtTileCoord(p) )
	{
		tmp->addControlPoint(p);
	}
	
	//BOTTOM RIGHT
	p = Point(tileCoord.x + 1,tileCoord.y + 1);
	if (b && r && isValidTileCoord(p) && !isWallAtTileCoord(p) )
	{
		tmp->addControlPoint(p);
	}
	return tmp;
}

bool HelloWorld::init()
{
	if ( !Layer::init() )
	{
		return false;
	}    

	std::string resources = "Map";        // partial paths are OK as resource paths.
	std::string file = resources + "/CatMaze.tmx";
	auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
	CCASSERT(str != NULL, "Unable to open file");
	_tileMap = TMXTiledMap::createWithXML(str->getCString() ,resources.c_str());  
	this->addChild(_tileMap);

	auto spawnTileCoord = Point(24,0);
	auto spawnPos = positionForTileCoord(spawnTileCoord);
	setViewPointCenter(spawnPos);

	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/pickup.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/catAttack.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/hitWall.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/lose.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/step.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/win.wav");

	_bgLayer = _tileMap->getLayer("Background");
	_objectLayer = _tileMap->getLayer("Objects");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map/CatMaze.plist");
	_batchNode = SpriteBatchNode::create("Map/CatMaze.png");
	_tileMap->addChild(_batchNode);
	_cat = CatSprite::CreateWithLayer(this);
	_cat->setPosition(spawnPos);
	_batchNode->addChild(_cat);


	_bonesCount = Label::createWithBMFont("fonts/Arial.fnt","Bones: 0");
	_bonesCount->setPosition(400,300);
	this->addChild(_bonesCount);

	auto lister = EventListenerTouchOneByOne::create();
	lister->setSwallowTouches(true);
	lister->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lister, this);

	this->scheduleUpdate();
	return true;
}

void HelloWorld::update( float dt )
{
	this->setViewPointCenter(_cat->getPosition());
}

bool HelloWorld::onTouchBegan( Touch *touch, Event *unused_event )
{
	if (_gameOver)
	{
		return false;
	}
	auto touchLocation = _tileMap->convertTouchToNodeSpace(touch);
	_cat->moveToward(touchLocation);
	return true;
	
}

HelloWorld::HelloWorld()
{
	_tileMap = nullptr;
	_bgLayer = nullptr;
	_objectLayer = nullptr;
	_batchNode = nullptr;
	_node = nullptr;
	_cat = nullptr;
	_gameOver = false;
	_won = false;
	_bonesCount = nullptr;
}
















