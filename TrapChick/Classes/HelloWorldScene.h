#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;


class HelloWorld;
class CircleSprite;
class Chicken;
class ShortestPathStep;

class CircleSprite: public Sprite
{
public:
	CircleSprite();
	static CircleSprite * createFromCoorAndLayer(Point cooor,HelloWorld * layer);
	bool  initWithCoorAndLayer(Point cooor,HelloWorld * layer);
	Point getCoor(){return _coor;}
	int   getId();
	bool  IsSelect(){return _isselect;}
	void  setSelect();
private:
	HelloWorld * _layer;
	Point        _coor;
	bool         _isselect;
};

class Chicken : public Sprite
{
public:
	Chicken();
	~Chicken();
	static Chicken * createFromLayer(HelloWorld * layer);
	bool initWithLayer(HelloWorld * layer);
	CC_SYNTHESIZE_RETAIN(Animation *,_normalAnimation,NormalAnimation);
	CC_SYNTHESIZE_RETAIN(Animation *,_trapAnimation,TrapAnimation);
	void runAnimation(Animation * animaton);
	Point getCoor(){return _coor;}
	void  setCoor(Point coor);
	bool  IsTrap(){return _isTrap;}
	void  setTrap();
private:
	Point      _coor;
	HelloWorld * _layer;
	Animation * _currentAnimation;
	Action  *  _curAnimate;
	bool       _isTrap;

};

class ShortestPathStep : public Ref
{
public:
	static ShortestPathStep * createFromPos(Point pos);
private:
	CC_SYNTHESIZE(Point,position,Pos);
	CC_SYNTHESIZE(int,gScore,GScore);
	CC_SYNTHESIZE(int,hScore,HScore);
	CC_SYNTHESIZE(ShortestPathStep *,parent,Parent);
public:
	bool initWithPositon(Point pos);
	bool isEqual(ShortestPathStep * other);
	int  getFScore();
	std::string getInfo();
};

class ComputePath : public Ref
{
public:
	static ComputePath * createFromLayer(HelloWorld * layer);
    void  getPath(Point from,Point to);
	~ComputePath();
	int   getStepCount(){return shortestPath.size();};
	Point getFirstStep(){return shortestPath.at(0)->getPos();}
private:
	HelloWorld * _layer;
	Vector<ShortestPathStep*> spOpenSteps;
	Vector<ShortestPathStep*> spClosedSteps;
	Vector<ShortestPathStep*> shortestPath;
	bool initWithLayer(HelloWorld * layer);
	void insertInOpenSteps(ShortestPathStep* step);
	int computeHScoreFromCoord(Point from,Point to);
	int costToMoveFromStep(ShortestPathStep* fromStep,ShortestPathStep* toStep);
	void constructPath(ShortestPathStep* step);

};




class HelloWorld : public cocos2d::Layer
{
public:
	enum class MoveType
	{
		LEFT = 0,
		RIGTH = 1,
		TOPLEFT = 2,
		TOPRIGHT = 3,
		BOTTOMLEFT = 4,
		BOTTOMRIGHT = 5
	};
	HelloWorld();
    static cocos2d::Scene* createScene();
	CREATE_FUNC(HelloWorld);
	Point getPositionFromCoor(Point coor);
	CircleSprite * getSpriteFromCoor(Point coor);
	void addCircle(CircleSprite * circle);
	void  addCost(Point coor);
	PointArray * getValidStep(Point coor);
private:
	virtual bool init();
	void initCircle();
	virtual bool onTouchBegan( Touch *touch, Event *unused_event );
	void showMoves(int num);
	void addMove();
	bool checkIsLoseFromCoor(Point coor);
	Point getTarget(Point coor, MoveType movetype);
	void  showRestartMenu(); 
	void  restartTapped(Ref * ref); 

	

private:
	map<int,CircleSprite*> _circles;
	Chicken * _chicken;
	bool        _gameOver;
	bool        _won;
	Label      *_moveCountLabel; 
	int         _moveCount;  
	float         _left;
	float         _right;
	float         _up;
	float         _down;
};





#endif // __HELLOWORLD_SCENE_H__
