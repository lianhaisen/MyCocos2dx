#ifndef  _CATSPRITE_H_
#define  _CATSPRITE_H_

#include "cocos2d.h"
USING_NS_CC;
using namespace std;


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


class HelloWorld;
class CatSprite : public Sprite
{
public:
	CatSprite();
	~CatSprite();
private:
	Animation *  createCatAnimation(std::string animType);
	void  runAnimation(Animation * animation);
private:
	CC_SYNTHESIZE_RETAIN(Animation  *,_facingForwardAnimation,FacingForward);
	CC_SYNTHESIZE_RETAIN(Animation  *,_facingBackAnimation,FacingBack);
	CC_SYNTHESIZE_RETAIN(Animation  *,_facingLeftAnimation,FacingLeft);
	CC_SYNTHESIZE_RETAIN(Animation  *,_facingRightAnimation,FacingRight);
	Animation  * _curAnimation;
	Action    * _curAnimate;
	int        _numBones;
	Action * currentStepAction;
	Point  pendingMove;
	HelloWorld * _layer;
	unsigned int currentPlayedEffect; 
	bool _isMoving;
public:
	static CatSprite * CreateWithLayer(HelloWorld * layer);
    int getNumBones(){return _numBones;}
	bool initWithLayer(HelloWorld * layer);
	void moveToward(Point target);
	//»ñµÃÂ·¾¶
	void getPath(Point target);
	void update(float dt);
	void onMoveCallBack();

private:
	Vector<ShortestPathStep*> spOpenSteps;
	Vector<ShortestPathStep*> spClosedSteps;
	Vector<ShortestPathStep*> shortestPath;
   void insertInOpenSteps(ShortestPathStep* step);
   int computeHScoreFromCoord(Point from,Point to);
   int costToMoveFromStep(ShortestPathStep* fromStep,ShortestPathStep* toStep);
   void constructPathAndStartAnimationFromStep(ShortestPathStep * step);
   void popStepAndAnimate();
  


};

#endif



