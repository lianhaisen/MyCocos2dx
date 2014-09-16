#include "UIRecruitHeroHerdLayer.h"
#include "AppMacros.h"
#define CON Configuration::getInstance()

UIRecruitHeroHerdLayer::UIRecruitHeroHerdLayer()
{
	m_Headbtn = nullptr;
	m_TopName = nullptr;
	m_DownName = nullptr;
	_callBack = nullptr;
}
UIRecruitHeroHerdLayer::~UIRecruitHeroHerdLayer()
{
	_callBack = nullptr;
	/*CC_SAFE_RELEASE(m_Headbtn);
	CC_SAFE_RELEASE(m_TopName);
	CC_SAFE_RELEASE(m_DownName);*/
	
}

UIRecruitHeroHerdLayer* UIRecruitHeroHerdLayer::create(int nHeroId,RecruitType nType)
{
	UIRecruitHeroHerdLayer *pRet = new UIRecruitHeroHerdLayer();
	if(pRet && pRet->init(nHeroId,nType))
	{
		pRet->autorelease();

		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;

}

 bool UIRecruitHeroHerdLayer::init(int nHeroId,RecruitType nType)
 {
	 if (Layer::init())
	{   

		m_nType = nType;
		initWithForm(nHeroId);
		return true;
	}
	 return false;
 }

 void UIRecruitHeroHerdLayer::initWithForm(int nHeroId)
 {
	 auto ve = HeroInfo::getHeroConfig();
	 map<int,HeroInfo*>::iterator it = ve.find(nHeroId);
	 if(it != ve.end()){
		 _db = it->second;
	 }
	 else{
		 return ;
	 }
	 int iNum = _db->id%10;
	 if(iNum > 3)
		 iNum = 3;
	 auto Path = String::createWithFormat("hero/hero%02d.png",iNum);
	 auto strHeadPath = String::createWithFormat("%s","form/form004.png")->getCString();
	 float Scale = 0.1f;

	 if(m_nType == RecruitType::SMALL){
		 Path = String::createWithFormat("hero/hero%02d_head.png",iNum);
		 strHeadPath = String::createWithFormat("%s","btn/btn_hero_head01.png")->getCString();
		 Scale = 0.25f;
	 }
	 m_Headbtn = UITouchButton::create(strHeadPath);
	 m_Headbtn->setClickTarget(CC_CALLBACK_1(UIRecruitHeroHerdLayer::CallBack,this));
	 this->setContentSize(m_Headbtn->getContentSize());
	 this->addChild(m_Headbtn);
	 m_Headbtn->setIsUsedForScroller(true);
	 m_Headbtn->setPosition(m_Headbtn->getContentSize().width/2,m_Headbtn->getContentSize().height/2);

	 auto spHrad = Sprite::create(Path->getCString());
	 m_Headbtn->addChild(spHrad);
	 spHrad->setScale(m_Headbtn->getContentSize().width/spHrad->getContentSize().width-Scale);

	//auto Name = String::createWithFormat("Lv.%d",12);
	 m_TopName = Label::createWithSystemFont(_db->name,"",24);
	m_TopName->setAnchorPoint(Point(0.5,0));
	m_TopName->setPosition(m_Headbtn->getContentSize().width/2,m_Headbtn->getContentSize().height);
	this->addChild(m_TopName);

	//auto lv = String::createWithFormat("%04d",12);
	m_DownName = Label::createWithSystemFont(" ","",24);
	m_DownName->setAnchorPoint(Point(0.5,0));
	m_DownName->setPosition(m_Headbtn->getContentSize().width/2,-m_DownName->getContentSize().height);
	this->addChild(m_DownName);
 }

void UIRecruitHeroHerdLayer::setTopName(const string str)
{
	m_TopName->setString(str);
}

void UIRecruitHeroHerdLayer::setDownName(const string str)
{
	m_DownName->setString(str);
}

void UIRecruitHeroHerdLayer::CallBack(Ref* sender)
{
	if(_callBack)
	{
		_callBack(this);
	}
}
void UIRecruitHeroHerdLayer::setCallBack(const ccMenuCallback& callback)
{
	_callBack = callback;
}

