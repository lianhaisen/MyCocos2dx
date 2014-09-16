#include "UISoldierHeadLayer.h"


UISoldierHeadLayer::UISoldierHeadLayer()
{
	m_Soldier = NULL;
	m_RecruitSoldierCallBack = nullptr;
	_data = nullptr;
	m_btnTop = nullptr;
}

UISoldierHeadLayer::~UISoldierHeadLayer()
{
	CC_SAFE_DELETE(m_Soldier);
}

bool UISoldierHeadLayer::init(CSoldier * data ,int nNum,bool v)
{
	if (Layer::init())
	{   
		//添加背景
		_data = data;
		m_Soldier = new Soldierinfo();
		m_Soldier->nType = data->id/100;
		m_Soldier->nNum = nNum;
		m_Soldier->nGold = data->train_cost;
		m_Soldier->Lv = data->id % 100;
		m_Soldier->nTime = data->train_time;
		m_Soldier->Path = String::createWithFormat("ui/%d.png",data->id/100)->getCString();
		m_Soldier->bIsRecruit = v;
		initWithFormat();
		return true;
	}


	return false;
}
UISoldierHeadLayer* UISoldierHeadLayer::create(CSoldier * data ,int nNum,bool v)
{
	UISoldierHeadLayer *pRet = new UISoldierHeadLayer();
	if(pRet && pRet->init(data,nNum,v))
	{
		pRet->autorelease();

		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void UISoldierHeadLayer::initWithFormat()
{
	m_Soldier->btn =  UITouchButton::create("btn/btn_hero_head01.png");
	m_Soldier->imgmiddle = Sprite::create("ui/1192.0.png");
	this->setContentSize(m_Soldier->btn->getContentSize());
	m_Soldier->btn->setClickTarget(CC_CALLBACK_1(UISoldierHeadLayer::addRecruitSoldier,this));
	m_Soldier->btn->setIsNeedLongTouch(true);
	m_Soldier->btn->setLongPressTarget(CC_CALLBACK_1(UISoldierHeadLayer::addRecruitSoldier,this));

	//头像
	m_Soldier->imgHerd = Sprite::create(m_Soldier->Path);
	m_Soldier->imgHerd->setScale(1.1f);
	m_Soldier->btn->addChild(m_Soldier->imgHerd);
	//提示LOG
	auto btnTop = UITouchButton::create("ui/69.0.png");
	btnTop->setPosition(btnTop->getContentSize().width*1.5,btnTop->getContentSize().height*1.5);
	btnTop->setClickTarget(CC_CALLBACK_1(UISoldierHeadLayer::onClickCallBack,this));
	m_Soldier->btn->addChild(btnTop,0,m_Soldier->nType);
	m_btnTop = btnTop;
	//已有数量
	auto nNum = String::createWithFormat("%d",m_Soldier->nNum);
	m_Soldier->lbNum = Label::createWithSystemFont(nNum->getCString(),"",24);
	m_Soldier->lbNum->setAnchorPoint(Point(0,1));
	m_Soldier->lbNum->setPosition(0,m_Soldier->imgHerd->getContentSize().height);
	m_Soldier->imgHerd->addChild(m_Soldier->lbNum);
	//士兵等级
	auto mask = Sprite::create("ui/mask_01.png");
	mask->setAnchorPoint(Point(0.5,0));
	mask->setPosition(m_Soldier->imgHerd->getContentSize().width/2,-1);
	m_Soldier->imgHerd->addChild(mask);
	auto lv = String::createWithFormat("Lv.%d",m_Soldier->Lv);
	m_Soldier->lbLv = Label::createWithSystemFont(lv->getCString(),"",24);
	m_Soldier->lbLv->setAnchorPoint(Point(0.5,0.5));
	m_Soldier->lbLv->setPosition(mask->getContentSize().width/2,mask->getContentSize().height/2);
	mask->addChild(m_Soldier->lbLv);

	_DownLayer = Node::create();

	//需要金币
	auto str = String::createWithFormat("%d",m_Soldier->nGold);
	m_Soldier->lbGold = Label::createWithSystemFont(str->getCString(),"",24);

	auto imgGold = Sprite::create("ui/blue_crystal.png");
	float Scale = m_Soldier->lbGold->getContentSize().height/imgGold->getContentSize().height;
	imgGold->setScale(Scale);
	imgGold->setAnchorPoint(Point(0.5,1));
	imgGold->setPosition(-m_Soldier->lbGold->getContentSize().width/2,0);
	_DownLayer->addChild(imgGold);

	_DownLayer->setAnchorPoint(Point(0,0));
	_DownLayer->setPosition(0,-imgGold->getContentSize().height);
	this->addChild(_DownLayer);

	m_Soldier->lbGold->setAnchorPoint(Point(0.5,1));
	m_Soldier->lbGold->setPosition(imgGold->getContentSize().width/2*Scale,imgGold->getPositionY());
	_DownLayer->addChild(m_Soldier->lbGold);

	//置灰图片；不可招募时
	auto imgHerd2 = Sprite::create(m_Soldier->Path);
	imgHerd2->setScale(1.1f);
	m_Soldier->imgmiddle->addChild(imgHerd2,0,10);
	imgHerd2->setOpacity(80);
	imgHerd2->setColor(Color3B(100,100,100));

	this->addChild(m_Soldier->btn,10,m_Soldier->nType+100);
	this->addChild(m_Soldier->imgmiddle);
	setIsRecruit(m_Soldier->bIsRecruit);
}

void UISoldierHeadLayer::setIsRecruit(bool b)
{
	//可否招募
	m_Soldier->bIsRecruit = b;
	if(m_Soldier->bIsRecruit){
		m_Soldier->imgmiddle->setVisible(false);
		m_Soldier->btn->setVisible(true);
	}
	else{
		m_Soldier->imgmiddle->setVisible(true);
		m_Soldier->btn->setVisible(false);
	}

}


void UISoldierHeadLayer::setRecruitCallBack(const ccMenuCallback& callback)
{
	m_RecruitSoldierCallBack = callback;
}

void UISoldierHeadLayer::addRecruitSoldier(Ref* sender)
{
	if(m_RecruitSoldierCallBack)
	{
		m_RecruitSoldierCallBack(this);
	}
}

void UISoldierHeadLayer::setTipCallBack(const ccMenuCallback& callback)
{
	m_TipCallBack = callback;
}

void UISoldierHeadLayer::onClickCallBack(Ref* sender)
{
	if(m_TipCallBack)
	{
		m_TipCallBack(this);
	}
}

void UISoldierHeadLayer::setIsTopVisible( bool v )
{
	if(m_btnTop)
	{
		m_btnTop->setVisible(v);
	}
}
