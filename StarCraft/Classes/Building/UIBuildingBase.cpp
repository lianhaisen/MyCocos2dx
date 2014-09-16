#include "UIBuildingBase.h"
#include "BuildingCommon.h"
#include "BuildingData.h"
#include "BuildingConfig.h"
#include "AppMacros.h"
#include "UIPlaceBuildingLayer.h"

UIBuildingBase::UIBuildingBase()
{
	_grassbg = nullptr;
	_buildingArmature = nullptr;
	_bigeraction = nullptr;
	_sharkaction = nullptr;
	_arr1 = nullptr;
	_arr2 = nullptr;
	_arr3 = nullptr;
	_arr4 = nullptr;
	_processbg = nullptr;
	_processTimer = nullptr;
	_btnyes = nullptr;
	_btnno = nullptr;
	_hpProcess = nullptr;
	_hpgb = nullptr;
	_buildingproc = nullptr;

}

UIBuildingBase::~UIBuildingBase()
{
	CC_SAFE_RELEASE(_grassbg);
	CC_SAFE_RELEASE(_buildingArmature);
	CC_SAFE_RELEASE(_bigeraction);
	CC_SAFE_RELEASE(_sharkaction);
	CC_SAFE_RELEASE(_arr1);
	CC_SAFE_RELEASE(_arr2);
	CC_SAFE_RELEASE(_arr3);
	CC_SAFE_RELEASE(_arr4);
	CC_SAFE_RELEASE(_processbg);
	CC_SAFE_RELEASE(_processTimer);
	CC_SAFE_RELEASE(_btnyes);
	CC_SAFE_RELEASE(_btnno);
	CC_SAFE_RELEASE(_hpProcess);
	CC_SAFE_RELEASE(_buildingproc);

}

void UIBuildingBase::setSharkAction()
{
	ActionInterval * ac = TintBy::create(1,-100,-100,-100);
	ActionInterval * acback = ac->reverse();
	ActionInterval * final = RepeatForever::create(Sequence::create(ac,acback,NULL));
	setSharkAction(final);
}

void UIBuildingBase::setBigerAction()
{
	float scale = getBuildingArmature()->getScale();
	ActionInterval * final = Sequence::create(CCScaleTo::create(0.1f,scale + scale * 0.1f),
		ScaleTo::create(0.05f,scale),
		CallFunc::create(CC_CALLBACK_0(UIBuildingBase::bigActionBack,this)),
		NULL
		);
	setBigAction(final);
}

void UIBuildingBase::bigActionBack()
{
	getBuildingArmature()->runAction(getSharkAction());
}

void UIBuildingBase::setCoor( Coord coor )
{
	_coor = coor;
	auto pos = MapHelper::getPointFromCoord(coor.x,coor.y);
	this->setPosition(pos);
}

void UIBuildingBase::updateUpgradeStatus()
{
	if (!getProcessTimer())
	{
		createProcessTimer();
		createBuildProc();
		scheduleUpdate();
	}
	else
	{
		//移除
		getProcessBg()->removeFromParent();
		getProcessTimer()->removeFromParent();
		setProcessTimer(nullptr);
		setProcessBg(nullptr);
		getBuildingProc()->removeFromParent();
		setBuildingProc(nullptr);
		unscheduleUpdate();
		updateStatus();
	}
}

void UIBuildingBase::createProcessTimer()
{
	setProcessBg(Sprite::create("ui/processbg.png"));
	addChild(getProcessBg());
	getProcessBg()->setPosition(Point(getContentSize().width / 2 ,getContentSize().height + 20));
	Sprite* timer = Sprite::create("ui/process.png");
	setProcessTimer(ProgressTimer::create(timer));
	addChild(getProcessTimer());
	getProcessTimer()->setType(ProgressTimer::Type::BAR);
	getProcessTimer()->setMidpoint(Point(0,100));
	getProcessTimer()->setPosition(Point(getContentSize().width / 2 ,getContentSize().height + 20));
	getProcessTimer()->setBarChangeRate(Point(1,0));
}

void UIBuildingBase::createHPProcess()
{
	setHPBg(Sprite::create("ui/hp2.png"));
	addChild(getHPBg());
	getHPBg()->setPosition(Point(getContentSize().width / 2 ,getContentSize().height + 20));
	setHPProcess(ProgressTimer::create(Sprite::create("ui/hp.png")));
	addChild(getHPProcess());
	getHPProcess()->setType(ProgressTimer::Type::BAR);
	getHPProcess()->setMidpoint(Point(0,100));
	getHPProcess()->setPosition(Point(getContentSize().width / 2 ,getContentSize().height + 20));
	getHPProcess()->setBarChangeRate(Point(1,0));
	getHPProcess()->setPercentage(100);
}

void UIBuildingBase::createBuildProc()
{
	if (getGridCount() >= 2)
	{
		setBuildingProc(Armature::create("building"));
		getBuildingProc()->getAnimation()->play(String::createWithFormat("01_building_%d",getGridCount())->getCString());
		getBuildingProc()->setScale(0.5f);
		getBuildingProc()->setPosition(Point(getContentSize().width /2,0));
		addChild(getBuildingProc());
	}
}

void UIBuildingBase::createBuyButton()
{
	setBtnBuyYES(UITouchButton::create("ui/yes.png"));
	setBtnBuyNo(UITouchButton::create("ui/no.png"));

	auto width  = getBtnBuyNo()->getContentSize().width ;
	auto height =  getBtnBuyNo()->getContentSize().height;

	getBtnBuyNo()->setPosition(Point(getContentSize().width / 4 - width / 2 ,getContentSize().height / 4 * 3  + height / 2));
	getBtnBuyYES()->setPosition(Point(getContentSize().width / 4 * 3 + width /2  ,getContentSize().height / 4 * 3 + height / 2 ));


	getBtnBuyNo()->setTag(0);
	getBtnBuyYES()->setTag(1);
	getBtnBuyYES()->setClickTarget(CC_CALLBACK_1(UIBuildingBase::onBuyDone,this));
	getBtnBuyNo()->setClickTarget(CC_CALLBACK_1(UIBuildingBase::onBuyDone,this));


	addChild(getBtnBuyYES());
	addChild(getBtnBuyNo());
}

bool UIBuildingBase::isPointInRect( const Point& point )
{
	bool result = false;
	Rect r = this->getBoundingBox();
	if (r.containsPoint(point))
	{
		result = true;
	}
	return result;
}

UIPlaceBuildingLayer* UIBuildingBase::getPlaceLayer()
{
	UIPlaceBuildingLayer * item = nullptr; 
	auto pa = this->getParent();
	if (pa)
	{
		item = dynamic_cast<UIPlaceBuildingLayer *>(pa);
	}
	return item;
}

void UIBuildingBase::addBuildPoint()
{
	auto layer = getPlaceLayer();
	if (layer)
	{
		layer->addbuildPoint(_coor.x,_coor.y,m_nGridCount);
	}
}



bool UIBuildingBase::checkaddPoint()
{
	bool result = false;
	auto layer = getPlaceLayer();
	if (layer)
	{
		result = layer->checkaddPoint(_coor.x,_coor.y,m_nGridCount);
	}
	return result;
}

int EditLogic::getLevel()
{
	if (_db)
	{
		_level = _db->type % 100;
	}
	return _level;
}

int EditLogic::getBStatus()
{
	if (_db)
	{
		_bstatus = (int)_db->status;
	}
	return _bstatus;
}

EditLogic::EditLogic()
{
	_Selected = false;
	_bMove = false;
	_db = nullptr;
	_buyCfg = nullptr;
	_isInBuy = false;
	_isillegal = false;
	_yesorno = nullptr;
	_callBack = nullptr;
	_type = 0;
	_level = 0;
	_bstatus = 0;
}

EditLogic::~EditLogic()
{
	_callBack = nullptr;
}

void EditLogic::setClickTarget( const ccMenuCallback& callback )
{
	_callBack = callback;
}

void Building::removeBuildPoint()
{

	auto layer = getPlaceLayer();
	if (layer && !_isillegal)
	{
		layer->removebuildingPoint(_coor.x,_coor.y,m_nGridCount);
	}
}

void Building::selected()
{
	this->setLocalZOrder((MAX_GRID_COUNT + 1) * (MAX_GRID_COUNT + 1));
	getArr1()->setVisible(true);
	getArr2()->setVisible(true);
	getArr3()->setVisible(true);
	getArr4()->setVisible(true);
	getBuildingArmature()->runAction(getBigAction());
	_Selected = true;
	_orginCoor = getCoor();
	if (_callBack)
	{
		_callBack(this);
	}
}

void Building::unselected()
{
	this->setLocalZOrder( MAX_GRID_COUNT *_coor.y + _coor.x );
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
	if (_isillegal)
	{   
		setCoor(_orginCoor);
		setillegal(false);	
	}
	if (getGrassBg())
	{
		getGrassBg()->setVisible(true);
	}
	addBuildPoint();
	if (_db && _db->coord != _coor)
	{
		_db->updateCoor(_coor.x,_coor.y);
	}
	
}

void Building::draw( Renderer *renderer, const kmMat4 &transform, bool transformUpdated )
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(Building::onDraw, this, transform, transformUpdated); 
	renderer->addCommand(&_customCommand);
}


bool Building::isNeedShowGrid()
{
	return  _isInBuy || (_bMove && isSelected()) || _isillegal;
}

void Building::onDraw( const kmMat4 &transform, bool transformUpdated )
{
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);

	if (isNeedShowGrid())
	{  
		Size size = getContentSize();
		Point p1 = Point(size.width/2, size.height);
		Point p2 = Point(0, size.height/2);
		Point p3 = Point(size.width/2, 0);
		Point p4 = Point(size.width, size.height/2);
		Point points[4] = {p1, p2, p3, p4};

		if (_isillegal)
		{
			DrawPrimitives::drawSolidPoly(points, 4, Color4F(1,0,0,1));
		}
		else
		{
			DrawPrimitives::drawSolidPoly(points, 4, Color4F(1,1,0,1));
		}



		float w = (p1.x-p2.x)/m_nGridCount;
		float h = (p1.y-p2.y)/m_nGridCount;

		DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
		for (int i = 0; i <= m_nGridCount; i++)
		{
			DrawPrimitives::drawLine(Point(p1.x + w*i, p1.y - h*i), Point(p2.x + w*i, p2.y - h*i));
		}
		for (int i = 0; i <= m_nGridCount; i++)
		{
			DrawPrimitives::drawLine(Point(p1.x - w*i, p1.y - h*i), Point(p4.x - w*i, p4.y - h*i));
		}
	}

	kmGLPopMatrix();
}

void Building::setMove( bool isMove )
{
	if (isMove != _bMove)
	{  
		if (!_isillegal)
		{
			_orginCoor = _coor;
		}
		if (isMove)
		{   
			if (getGrassBg())
			{
				getGrassBg()->setVisible(false);
			}	
		}
		else
		{   
			if (!_isInBuy && !_isillegal)
			{   //购买状态下草皮不显示
				if (getGrassBg())
				{
					getGrassBg()->setVisible(true);
				}
			}
		}
	}
	_bMove = isMove;
}

void Building::setillegal( bool illeagal )
{
	if (illeagal != _isillegal)
	{   
		//购买模式下按钮显示不显示
		if (_isInBuy)
		{    
			if (illeagal)
			{
				getBtnBuyYES()->setVisible(false);	
			}
			else
			{
				getBtnBuyYES()->setVisible(true);	
			}
		}
	}
	_isillegal = illeagal;
}

void Building::updateStatus()
{
	if (getBuildingArmature() != nullptr)
	{    
		auto str = String::createWithFormat("%02d_nor",getLevel())->getCString();
		getBuildingArmature()->getAnimation()->play(str,-1,1);
	}
}

void Building::setBuy( bool isBuy /*= false*/,const ccYesOrNoCallBack& callback /*= nullptr */ )
{
	if (_isInBuy != isBuy)
	{   
		_yesorno = callback;
		if (isBuy)
		{   

			getArr1()->setVisible(true);
			getArr2()->setVisible(true);
			getArr3()->setVisible(true);
			getArr4()->setVisible(true);
			createBuyButton();
		}
		else
		{   

			getBtnBuyYES()->removeFromParent();
			getBtnBuyNo()->removeFromParent();
			setBtnBuyYES(nullptr);
			setBtnBuyNo(nullptr);

			getArr1()->setVisible(false);
			getArr2()->setVisible(false);
			getArr3()->setVisible(false);
			getArr4()->setVisible(false);
		}

	}
	_isInBuy = isBuy;
}

void Building::onBuyDone( Ref * ref )
{
	auto btn = dynamic_cast<UITouchButton*>(ref);
	if (getGrassBg())
	{
		getGrassBg()->setVisible(true);
	}
	if (_yesorno)
	{   
		_yesorno(this,btn->getTag() == 1);
	}
}

void Building::setBuidingData( DBuilding * db )
{
	_db = db;
	//TODO 设置建筑物
	_db->setBuilding(this);

	if (_db != nullptr && _db->getLeftTime() != 0)
	{   
		updateUpgradeStatus();
	}

}

bool Building::initWithFile( const char* filename,const int gridCount )
{   
	if (gridCount >= 2)
	{
		setGrassBg(Sprite::create(BuildingCommon::getGrassNameFromGridCount(gridCount)));
		getGrassBg()->setScale(0.5f);
		addChild(getGrassBg(),-1);
	}


	setBuildingArmature(Armature::create(filename));
	getBuildingArmature()->setScale(0.5f);
	addChild(getBuildingArmature());

	ignoreAnchorPointForPosition(false);
	setAnchorPoint(Point(0.5, 0));

	m_nGridCount = gridCount;

	setContentSize(Size(UNIT_WIDTH*2*gridCount*2, UNIT_HEIGHT*2*gridCount*2));
	if (getGrassBg())
	{
		getGrassBg()->setPosition(Point(getContentSize().width /2,getContentSize().height / 2));
	}
	getBuildingArmature()->setPosition(Point(getContentSize().width /2,0));

	setArr1(CCSprite::create("ui/arr1.png"));
	setArr2(CCSprite::create("ui/arr2.png"));
	setArr3(CCSprite::create("ui/arr3.png"));
	setArr4(CCSprite::create("ui/arr4.png"));

	Size arrsize = getArr1()->getContentSize();
	//log("%f %f",arrsize.width,arrsize.height);

	getArr1()->setPosition(Point(getContentSize().width / 4 * 3 + arrsize.width / 4,getContentSize().height / 4 * 3 +  arrsize.height /4 ));
	getArr2()->setPosition(Point(getContentSize().width / 4 - arrsize.width / 4,getContentSize().height / 4 * 3 +  arrsize.height /4 ));
	getArr3()->setPosition(Point(getContentSize().width  / 4 * 3 + arrsize.width / 4 ,getContentSize().height / 4 - arrsize.height /4 ));
	getArr4()->setPosition(Point(getContentSize().width / 4 - arrsize.width / 4 ,getContentSize().height / 4 - arrsize.height /4));

	addChild(getArr1());
	addChild(getArr2());
	addChild(getArr3());
	addChild(getArr4());

	getArr1()->setVisible(false);
	getArr2()->setVisible(false);
	getArr3()->setVisible(false);
	getArr4()->setVisible(false);

	setSharkAction();
	setBigerAction();

	return true;
}

void Building::update( float delta )
{
	if( delta > 1.0f / 59 )
		return;

	if (getProcessTimer() && getBuidingData() != nullptr)
	{   
		int cur = getBuidingData()->getLeftTime();
		if (cur == 0)
		{
			//TODO 移除滚动条
			updateUpgradeStatus();
		}
		else
		{   
			int total = 0;
			if (getBuidingData()->status == DBuilding::BuildingStatus::UPGRADE)
			{
				total =  getBuidingData()->getNextBindCfg()->cost_time;
			}
			else if (getBuidingData()->status == DBuilding::BuildingStatus::NewBuild)
			{
				total = getBuidingData()->getCurBindCfg()->cost_time;
			}
			getProcessTimer()->setPercentage((float)(total - cur) / total * 100);
		}
	}
}

bool Building::initWithDB( DBuilding * db )
{
	_type  = db->type / 100;
	_level = db->type % 100;

	if (initWithFile(BuildingCommon::getArmatureNameFromType(_type).c_str(),db->getCurBindCfg()->grid % 10))
	{  
		setBuidingData(db);
		setCoor(db->coord);
		updateStatus();
		return true;
	}
	return false;
}

bool Building::initWithBuyDB( BuyListConfig * db )
{
	_type  = db->buildcfg->id / 100;
	_level = db->buildcfg->id % 100;
	if (initWithFile(BuildingCommon::getArmatureNameFromType(_type).c_str(),db->buildcfg->grid % 10))
	{    
		_buyCfg = db;
		if (getGrassBg())
		{
			getGrassBg()->setVisible(false);
		}
		updateStatus();
		return true;
	}
	return false;
}

/****************************************************************************************************
战斗相关-begin
****************************************************************************************************/
bool Building::initWithBattleLogic( DBuilding * db )
{
	_type  = db->type / 100;
	_level = db->type % 100;
	setId(db->id);
	setType(db->type);
	setStatus(e_battle_status_idle);
	auto config = BuildingConfig::getBuildingConfig(db->type);
	setCoord(db->coord);
	//设置中心点
	setMidCoord(Coord(db->coord.x - config->grid,db->coord.y - config->grid));
	setHp(config->max_hp);
	if (initWithFile(BuildingCommon::getArmatureNameFromType(_type).c_str(),config->grid % 10))
	{  
		setCoor(db->coord);
		updateStatus();
		return true;
	}
	return false;
}

bool Building::subHp( int hp )
{
	updateHpProgress();
	return BattleLogic::subHp(hp);
}

void Building::updateHpProgress()
{
	if (getHPProcess() == nullptr)
	{
		createHPProcess();
	}
	auto cfg = BuildingConfig::getBuildingConfig(getType());
	float per = (float)getHp()/cfg->max_hp*100;
	getHPProcess()->setPercentage(per);
}

void Building::dead()
{
	setStatus(e_battle_status_dead);
	this->setVisible(false);
}

Coord Building::getMidCoord()
{    
	 return _midcoord;
}








/****************************************************************************************************
战斗相关-end
****************************************************************************************************/







