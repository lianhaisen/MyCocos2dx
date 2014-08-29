#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class CatSprite;
class HelloWorld : public cocos2d::Layer
{
public:
	HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	TMXTiledMap * _tileMap;
	TMXLayer    * _bgLayer;
	TMXLayer    * _objectLayer;
	SpriteBatchNode * _batchNode;
	Sprite      * _node;
	CatSprite   * _cat;
	bool        _gameOver;
	bool        _won;
	Label      * _bonesCount;
public:

	bool isValidTileCoord(Point tileCoord);
	Point tileCoordForPosition(Point position);
	Point positionForTileCoord(Point tileCoord);
	bool isProp(std::string prop,Point tileCoord,TMXLayer * layer);
	bool  isWallAtTileCoord(Point tileCoord);
	bool  isBoneAtTilecoord(Point tileCoord);
	bool  isDogAtTilecoord(Point tileCoord);
	bool  isExitAtTilecoord(Point tileCoord);
	void  removeObjectAtTileCoord(Point tileCoord);
	void  setViewPointCenter(Point pos);
	void  restartTapped(Ref * ref);
	void  showRestartMenu();
	void  endScene();
	void  winGame();
	void  loseGame();
	void  showNumBones(int num);
	PointArray * walkableAdjacentTilesCoordForTileCoord(Point tileCoord);
	virtual  void update(float dt);
	virtual bool onTouchBegan( Touch *touch, Event *unused_event );


};

#endif // __HELLOWORLD_SCENE_H__
