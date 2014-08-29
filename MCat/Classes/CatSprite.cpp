#include "CatSprite.h"
#include "HelloWorldScene.h"
using namespace CocosDenshion;

static const float kMovingSpeed = 0.4f;

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



Animation * CatSprite:: createCatAnimation(std::string animType)
{
	auto animation = Animation::create();
	for (int i = 1; i <= 2; i++)
	{   
		auto str = String::createWithFormat("cat_%s_%d.png",animType.c_str(),i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str->getCString());
		animation->addSpriteFrame(frame);
	}
	animation->setDelayPerUnit(0.2f);
	return animation;
}

void CatSprite::runAnimation( Animation * animation )
{
	if (_curAnimation == animation)
	{
		return;
	}
	_curAnimation = animation;
	if (_curAnimate != nullptr)
	{
		this->stopAction(_curAnimate);
	}
	_curAnimate = RepeatForever::create(Animate::create(animation));
	this->runAction(_curAnimate);


}

bool CatSprite::initWithLayer( HelloWorld * layer )
{   
	if (Sprite::initWithSpriteFrameName("cat_forward_1.png"))
	{
		_layer = layer;
		setFacingForward(createCatAnimation("forward"));
		setFacingBack(createCatAnimation("back"));
		setFacingLeft(createCatAnimation("left"));
		setFacingRight(createCatAnimation("right"));
	    this->scheduleUpdate();
	}

	return true;
}

CatSprite::CatSprite()
{
	_layer = nullptr;
	_facingForwardAnimation = nullptr;
	_facingBackAnimation = nullptr;
	_facingLeftAnimation = nullptr;
	_facingRightAnimation = nullptr;
	_curAnimation = nullptr;
	_curAnimate = nullptr;
	_numBones = 0;
	currentStepAction = nullptr;
	pendingMove = Point::ZERO;
	_layer = nullptr;
	currentPlayedEffect = 0;
	_isMoving = false;
}

CatSprite::~CatSprite()
{
	CC_SAFE_RELEASE(_facingForwardAnimation);
	CC_SAFE_RELEASE(_facingBackAnimation);
	CC_SAFE_RELEASE(_facingLeftAnimation);
	CC_SAFE_RELEASE(_facingRightAnimation);
    
	spOpenSteps.clear();
	spClosedSteps.clear();
	shortestPath.clear();
	currentStepAction  = nullptr;

}

CatSprite * CatSprite::CreateWithLayer( HelloWorld * layer )
{
	auto item = new CatSprite();
	item->initWithLayer(layer);
	item->autorelease();
	return item;
}



void CatSprite::moveToward( Point target )
{   
	//停止当前动作
    pendingMove = target;
}

void CatSprite::insertInOpenSteps( ShortestPathStep* step )
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

int CatSprite::computeHScoreFromCoord( Point from,Point to )
{
	return abs(to.x - from.x) + abs(to.y - from.y);
}

int CatSprite::costToMoveFromStep( ShortestPathStep* fromStep,ShortestPathStep* toStep )
{
	auto value = (fromStep->getPos() != toStep->getPos()) ? 14:10;
	return value;
}

void CatSprite::constructPathAndStartAnimationFromStep( ShortestPathStep * step )
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

	popStepAndAnimate();
}

void CatSprite::popStepAndAnimate()
{   
	_isMoving = true;
	currentStepAction = nullptr;
	//是否有路可以走
	if (shortestPath.empty())
	{
		return;
	}
	//下一步要走的路

	auto currentpos = _layer->tileCoordForPosition(this->getPosition());
	auto s = shortestPath.at(0);
	auto futurePos = s->getPos();
    auto diff = futurePos - currentpos;
	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x > 0)
		{
			runAnimation(getFacingRight());
		}
		else
		{
			runAnimation(getFacingLeft());
		}
	}
	else
	{
		if (diff.y > 0)
		{
			runAnimation(getFacingForward());
		}
		else
		{
			runAnimation(getFacingBack());
		}
	}

	currentStepAction = Sequence::createWithTwoActions(
		                 MoveTo::create(kMovingSpeed,_layer->positionForTileCoord(s->getPos())),
						 CallFunc::create(CC_CALLBACK_0(CatSprite::onMoveCallBack,this))
		                 ) ;
		             
	shortestPath.erase(0);
	this->runAction(currentStepAction);


}

void CatSprite::update( float dt )
{
	if (pendingMove != Point::ZERO && !_isMoving)
	{   
		getPath(pendingMove);
		pendingMove = Point::ZERO;
		return;
	}
	if (!shortestPath.empty() && !_isMoving)
	{
		popStepAndAnimate();
		return;
	}

}

void CatSprite::getPath( Point target )
{   

	SimpleAudioEngine::getInstance()->stopEffect(currentPlayedEffect);
	currentPlayedEffect = 0;

	//初始化路径属性
	spOpenSteps.clear();
	spClosedSteps.clear();
	shortestPath.clear();

	//获得当前地图坐标
	auto fromTileCoor = _layer->tileCoordForPosition(this->getPosition());
	auto toTileCoord = _layer->tileCoordForPosition(target);

	//检查是否有路径可以计算
	if (fromTileCoor == toTileCoord)
	{
		return;
	}

	//如果点击的位置是墙壁的话
	if (_layer->isWallAtTileCoord(toTileCoord))
	{
		currentPlayedEffect = SimpleAudioEngine::getInstance()->playEffect("Sounds/hitWall.wav");
		return;
	}

	insertInOpenSteps(ShortestPathStep::createFromPos(fromTileCoor));
	do 
	{
		// Get the lowest F cost step
		// Because the list is ordered, the first step is always the one with the lowest F cost 
		auto currentStep = spOpenSteps.at(0);

		// Add the current step to the closed set 
		spClosedSteps.pushBack(currentStep);

		// Remove it from the open list
		// Note that if we wanted to first removing from the open list, care should be taken to the memory  
		spOpenSteps.erase(0);
		// If the currentStep is at the desired tile coordinate, we have done 
		if (currentStep->getPos() == toTileCoord)
		{
			constructPathAndStartAnimationFromStep(currentStep);
			spOpenSteps.clear();
			spClosedSteps.clear();
			break;
		}

		auto points = _layer->walkableAdjacentTilesCoordForTileCoord(currentStep->getPos());
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



			// not on the open list ,so add it
			if (index == -1)
			{
				step->setParent(currentStep);
				step->setGScore(currentStep->getGScore() + moveCost);
				step->setHScore(this->computeHScoreFromCoord(step->getPos(),toTileCoord));
				this->insertInOpenSteps(step);
			}
			//Already in the open list
			else
			{   
				// To retrieve the old one (which has its scores already computed ;-) 
				step = this->spOpenSteps.at(index);
				if (currentStep->getGScore() + moveCost < step->getGScore())
				{
					// The G score is equal to the parent G score + the cost to move from the parent to it 
					step->setGScore(currentStep->getGScore() + moveCost);
					// Because the G Score has changed, the F score may have changed too
					// So to keep the open list ordered we have to remove the step, and re-insert it with
					// the insert function which is preserving the list ordered by F score

					// We have to retain it before removing it from the list 
					step->retain();
					// Now we can removing it from the list without be afraid that it can be released 
					spOpenSteps.erase(index);

					this->insertInOpenSteps(step);

					step->release();
				}

			}



		}


	} while (spOpenSteps.size() > 0);

	//额米有找到路径
	if (shortestPath.empty())
	{
		currentPlayedEffect = SimpleAudioEngine::getInstance()->playEffect("Sounds/hitWall.wav");
	}
}

void CatSprite::onMoveCallBack()
{   
	auto currentpos = _layer->tileCoordForPosition(this->getPosition());
	if (_layer->isBoneAtTilecoord(currentpos))
	{
		currentPlayedEffect = SimpleAudioEngine::getInstance()->playEffect("Sounds/pickup.wav");
		_numBones++;
		_layer->showNumBones(_numBones);
		_layer->removeObjectAtTileCoord(currentpos);
	}
	else if (_layer->isDogAtTilecoord(currentpos))
	{
		if (_numBones == 0)
		{
			_layer->loseGame();
			shortestPath.clear();
			return;
		}
		else
		{
			_numBones --;
			_layer->showNumBones(_numBones);
			_layer->removeObjectAtTileCoord(currentpos);
			currentPlayedEffect = SimpleAudioEngine::getInstance()->playEffect("Sounds/catAttack.wav");
		}
	}
	else if (_layer->isExitAtTilecoord(currentpos))
	{
		_layer->winGame();
		shortestPath.clear();
		return;
	}
	else
	{
		currentPlayedEffect = SimpleAudioEngine::getInstance()->playEffect("Sounds/step.wav");
	}
	_isMoving = false;
}


