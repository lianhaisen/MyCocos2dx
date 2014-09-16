#include "CustomTableViewCell.h"

CustomTableViewCell::CustomTableViewCell()
{
	_bg = nullptr;
	_infoPic = nullptr;
	_title = nullptr;
	_des = nullptr;
	_lablecost = nullptr;
	_timeLable = nullptr;
	_hasbuildLable = nullptr;
	_istouched = false;
	_ismoving = false;
	_clickCallBack = nullptr;
	_enableBackground = nullptr;
	_enableTip = nullptr;
	_isdisable = false;
}

CustomTableViewCell::~CustomTableViewCell()
{
	CC_SAFE_RELEASE(_bg);
	CC_SAFE_RELEASE(_infoPic);
	CC_SAFE_RELEASE(_title);
	CC_SAFE_RELEASE(_des);
	CC_SAFE_RELEASE(_lablecost);
	CC_SAFE_RELEASE(_timeLable);
	CC_SAFE_RELEASE(_hasbuildLable);
	CC_SAFE_RELEASE(_enableBackground);
	CC_SAFE_RELEASE(_enableTip);
}

bool CustomTableViewCell::init()
{   
    setBg(UIGraySprite::create("ui/card.png"));

	auto high = getBg()->getContentSize().height;
	auto width = getBg()->getContentSize().width;


	getBg()->setPosition(Point( width / 2,high / 2));
    addChild(getBg());

	setContentSize(Size(width,high));
	setAnchorPoint(Point(0.5f,0.5f));

	setInfoPic(UIGraySprite::create("building/740001.png"));
	getInfoPic()->setPosition(Point(width / 2 ,high));
	getInfoPic()->setScale(0.5f);
	getBg()->addChild(getInfoPic());

	setTitle(Label::createWithSystemFont("Title","",30));
	getTitle()->setPosition(Point(width/2,high - 80));
	getTitle()->setColor(Color3B(0,0,0));
	getBg()->addChild(getTitle());
     
	setDes(Label::createWithSystemFont("testtesetest","",15));
	getDes()->setPosition(Point(width/2,high - 100));
	getDes()->setColor(Color3B(0,0,0));
	getBg()->addChild(getDes());

	setLableCost(Label::createWithSystemFont("100","",25));
	getLableCost()->setPosition(Point(width/2,high - 150));
	getLableCost()->setColor(Color3B(0,0,0));
	getBg()->addChild(getLableCost());

	auto con = Configuration::getInstance();
	auto lablel = Label::createWithSystemFont(con->getValue("lable_buildtime").asString(),"",10);
	lablel->setColor(Color3B(0,0,0));
	lablel->setPosition(Point(45,30));
	getBg()->addChild(lablel);


	auto labler = Label::createWithSystemFont(con->getValue("lable_buildnum").asString(),"",10);
	labler->setColor(Color3B(0,0,0));
	labler->setPosition(Point(width - 45,30));
	getBg()->addChild(labler);

	setTimeLable(Label::createWithSystemFont("100s","",10));
	getTimeLable()->setColor(Color3B(0,0,0));
	getTimeLable()->setPosition(Point(40,15));
	getBg()->addChild(getTimeLable());

	setHasBuildLable(Label::createWithSystemFont("0/1","",10));
	getHasBuildLable()->setColor(Color3B(0,0,0));
	getHasBuildLable()->setPosition(Point(width - 45,15));
	getBg()->addChild(getHasBuildLable());

	setEnableBackground(Sprite::create("ui/card2.png"));
	getEnableBackground()->setPosition(Point(width/2,getEnableBackground()->getContentSize().height / 2 + 10));
	getBg()->addChild(getEnableBackground());

	setEnableTip(Label::createWithSystemFont("testtesetest","",15));
	getEnableTip()->setPosition(Point(getEnableBackground()->getContentSize().width / 2,getEnableBackground()->getContentSize().height / 2));
	getEnableTip()->setColor(Color3B(0,0,0));
	getEnableBackground()->addChild(getEnableTip());

	getEnableBackground()->setVisible(false);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->onTouchBegan = CC_CALLBACK_2(CustomTableViewCell::onTouchesBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CustomTableViewCell::onTouchesMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CustomTableViewCell::onTouchesEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(CustomTableViewCell::onTouchCancelled,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	

	return true;
}

bool CustomTableViewCell::onTouchesBegan( Touch * touch, cocos2d::Event *event )
{   
	if (isTouchIn(touch))
	{
		_istouched = true;
		getBg()->runAction(ScaleTo::create(0.05f, 0.9f));
	}
	return true;
}

void CustomTableViewCell::onTouchesMoved( Touch * touch, cocos2d::Event *event )
{  
	if (_istouched)
	{   
		_ismoving = true;
		if (!isTouchIn(touch))
		{   
			getBg()->runAction(ScaleTo::create(0.05f, 1.0f));
			_istouched = false;
		}
	}
}

void CustomTableViewCell::onTouchesEnded( Touch * touch, cocos2d::Event *event )
{   
	
	if (_istouched)
	{   
	    
       if (!_ismoving)
       {
		   auto se =  Sequence::createWithTwoActions(ScaleTo::create(0.05f, 1.0f),
			   CallFunc::create(CC_CALLBACK_0(CustomTableViewCell::onCloseCallBack,this)));
		   getBg()->runAction(se);
       }
	   else
	   {
		   getBg()->runAction(ScaleTo::create(0.05f, 1.0f));
	   }
	 
	}
	_istouched = false;
	_ismoving  = false;
}

void CustomTableViewCell::onTouchCancelled( Touch * touch, cocos2d::Event *event )
{
	onTouchesEnded(touch,event);
}

bool CustomTableViewCell::isTouchIn( Touch * touch )
{
	Point local = convertToNodeSpace(touch->getLocation());
	Rect r = getBg()->getBoundingBox();
	if (r.containsPoint(local))
	{
		return true;
	}
	return false;
}

void CustomTableViewCell::onCloseCallBack()
{
	if (_clickCallBack)
	{
		_clickCallBack(this);
	}
}

void CustomTableViewCell::setDisable( bool isDisable /*= false*/,std::string str /*= ""*/ )
{
	if (!isDisable)
	{
		getEnableBackground()->setVisible(false);
		getBg()->setGray(false);
		getInfoPic()->setGray(false);
	}
	else
	{
		getEnableBackground()->setVisible(true);
		getEnableTip()->setString(str);
		getBg()->setGray(true);
		getInfoPic()->setGray(true);
	}
	_isdisable = isDisable;
}

