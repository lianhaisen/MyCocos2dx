#include "UIUserMinUi.h"
#include "AppMacros.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d::extension;
#include "UIHeroConfiguration.h"
#include "UIHeroPub.h"
#include "UIBarracks.h"
static UIUserMinUi* m_instance;

UIUserMinUi::UIUserMinUi()
{
	m_HeadLayer = nullptr;
	_lbLv = nullptr; //等级Lb
	_lbChat = nullptr; //聊天
	_btnChat = nullptr;
	_btnStaticWarCallBack = nullptr;
}

UIUserMinUi::~UIUserMinUi()
{
	m_vWarBtnUi.clear();
	m_vTopUi.clear();
	m_vRightUi.clear();
	m_HeadLayer = nullptr;
	_lbLv = nullptr; //等级Lb
	_lbChat = nullptr; //聊天
}

UIUserMinUi* UIUserMinUi::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = UIUserMinUi::create();
		return m_instance;
	}
	return m_instance;
}

bool UIUserMinUi::init()
{
	if ( !Layer::init() )
    {
        return false;
    }
	createUIScene();
	return true;
}


void UIUserMinUi::createUIScene()
{
	_bPopUp = true;
	createHerdUi();
	createTopUi();
	creatRightUi();
	createDownUi();
	createChat();
}

//头部ui按钮回调
void UIUserMinUi::onClickTopUiCallback(Ref* sender)
{

	auto visibleSize = Director::getInstance()->getVisibleSize(); //窗口大小
	auto item =  static_cast<MenuItem*>(sender);
	int tag = item->getTag();
	if(tag == 1 )
	{
		auto layer = UIBarracks::create();
		addChild(layer);
	}
	else if (tag == 2 )
	{

	}
	else if (tag == 3 )
	{
		static int num = 1000;
		num += 1;
		addTopUiNum(2,num);
		addRightUiNum(2,num);
	}
	
}

void UIUserMinUi::createTopUi()
{
	auto visibleSize = Director::getInstance()->getVisibleSize(); //窗口大小
	auto origin = Director::getInstance()->getVisibleOrigin();
	int FONT_SIZE =  atoi(FONT_SIZE_XL);
	std::string Path[3] = {"ui/diamond.png", "ui/protect_time.png", "ui/workers.png"};

	for(int i = 1; i <= 3; i++)
	{
		//头UI
		auto LayerTop = UIUserInfoLayer::create("ui/processbg4.png",1,1,Path[i-1]);
		LayerTop->setAnchorPoint(Point(0,1));
		this->addChild(LayerTop);
		LayerTop->setTag(50+i);
		LayerTop->setinitNum(0,(i+(5*i))*9999);
		LayerTop->setHeadAction(true);
		//头像
		auto imgHerd = LayerTop->getHerdImg();
		imgHerd->setPositionX(imgHerd->getPositionX()+imgHerd->getContentSize().width/2);
		auto iPosX = m_HeadLayer->getContentSize().width/2 + LayerTop->getContentSize().width * i + imgHerd->getContentSize().width/1.5*i;
		LayerTop->setPosition(Point(iPosX,m_HeadLayer->getPositionY())+origin);

		//名字
		auto lbName = LayerTop->getNameLayer();
		lbName->setVisible(false);
		
		//设置内容
		auto lbInfo =  LayerTop->getInfoString();

		auto imgExp = LayerTop->getExpLayer();
		imgExp->setVisible(false);

		//添加按钮 2457.0
		auto btnAddItem = UITouchButton::create("btn/btn_add.png");
		btnAddItem->setPosition(Point(LayerTop->getContentSize().width - btnAddItem->getContentSize().width/3,LayerTop->getContentSize().height/2));
		LayerTop->addChild(btnAddItem,10,i);
		btnAddItem->setClickTarget(CC_CALLBACK_1(UIUserMinUi::onClickTopUiCallback,this));
		m_vTopUi.pushBack(LayerTop);
	}

}

void UIUserMinUi::createHerdUi()
{
	auto visibleSize = Director::getInstance()->getVisibleSize(); //窗口大小
	auto origin = Director::getInstance()->getVisibleOrigin();
	int FONT_SIZE =  atoi(FONT_SIZE_XL);
	//头像
	auto HerditemLayer = UIUserInfoLayer::create("ui/process_herolvbg.png",1,1,"ui/headlv.png","ui/processblue.png");
	HerditemLayer->setAnchorPoint(Point(0,1));
	this->addChild(HerditemLayer);
	//玩家经验赋值
	HerditemLayer->setinitNum(100,100);

	//头像位置
	auto imgHerd = HerditemLayer->getHerdImg();
	imgHerd->setPositionX(imgHerd->getPositionX()+imgHerd->getContentSize().width/2);
	HerditemLayer->setPosition(Point(imgHerd->getContentSize().width/2+5,visibleSize.height-imgHerd->getContentSize().height*1.5)+origin);

	auto lbLv = Label::createWithSystemFont("17","",FONT_SIZE);
	lbLv->setAnchorPoint(Point(0.5,0.5));
	lbLv->setPosition(Point(imgHerd->getContentSize().width/2,imgHerd->getContentSize().height/2) + origin);
	imgHerd->addChild(lbLv);
	_lbLv = lbLv;

	//设置名字
	auto lbName = HerditemLayer->getNameLayer();
	lbName->setString("Temp");
	lbName->setSystemFontSize(FONT_SIZE*1.2);
	lbName->setColor(Color3B(0,0,255));

	//设置内容
	auto lbInfo = HerditemLayer->getInfoString();
	lbInfo->setVisible(false);

	m_HeadLayer = HerditemLayer;
}

void UIUserMinUi::creatRightUi()
{
	////m_HeadLayer
	auto visibleSize = Director::getInstance()->getVisibleSize(); //窗口大小
	auto origin = Director::getInstance()->getVisibleOrigin();
	int FONT_SIZE =  atoi(FONT_SIZE_XL);
	std::string Path[4] = {"ui/metal.png", "ui/blue_crystal.png", "ui/black_crystal.png", "ui/energy.png"};
	std::string Path2[4] = {"ui/process_metal.png","ui/process_blue_crystal.png","ui/process_black_crystal.png","ui/process_energy.png"};
	for(int i = 1; i <= 4; i++)
	{
		//头UI
		std::string str = "ui/process_long.png";
		if(i > 2)
		{
			str = "ui/process_short.png";
		}
		auto LayerTop = UIUserInfoLayer::create(str,1,1,Path[i-1],Path2[i-1]);
		LayerTop->setAnchorPoint(Point(1,1));
		this->addChild(LayerTop);
		LayerTop->setTag(100+i);
		LayerTop->setinitNum(0,(i+(5*i))*1000);
		LayerTop->setHeadAction(true);

		auto iPosX = visibleSize.width -LayerTop->getContentSize().width ;
		auto iPosY = m_HeadLayer->getPositionY()-(LayerTop->getContentSize().height*(i-1)*2);
		LayerTop->setPosition(Point(iPosX,iPosY)+origin);

		//头像
		auto imgHerd = LayerTop->getHerdImg();
		imgHerd->setPosition(Point(LayerTop->getContentSize().width-imgHerd->getContentSize().width/2,LayerTop->getContentSize().height/2));

		//名字
		char string[128] = {0};
		sprintf(string, "%s%d", "Max:",LayerTop->getMaxNum());
		LayerTop->setName(string);

		auto lbName = LayerTop->getNameLayer();
		lbName->setSystemFontSize(18);
		lbName->setAnchorPoint(Point(0,0));
		lbName->setPosition(Point(0,LayerTop->getContentSize().height));


		auto imgExp = LayerTop->getExpLayer();
		imgExp->setMidpoint(Point(1,0));

		m_vRightUi.pushBack(LayerTop);
	}
}

void UIUserMinUi::createDownUi()
{
	auto visibleSize = Director::getInstance()->getVisibleSize(); //窗口大小
	auto origin = Director::getInstance()->getVisibleOrigin();
	int FONT_SIZE =  atoi(FONT_SIZE_XL);
	 //建造按钮
	 auto btn = UITouchButton::create("btn/btn_war.png");
	 //auto label = Label::createWithSystemFont("build","",20);
	 btn->setPosition(Point(visibleSize.width-btn->getContentSize().width/2-5 ,btn->getContentSize().height/2)+origin);
	 btn->setClickTarget(CC_CALLBACK_1(UIUserMinUi::onClickDownUiCallbacke,this));
	 //btn->addChild(label);
	 addChild(btn);
	 string str[4];
	 str[0] = "btn/btn_pk.png";
	 str[1] = "btn/btn_starwar.png";
	 str[2] = "btn/btn_npc.png";
	 str[3] = "btn/btn_friendly.png";
	 for(int i = 0; i < 4 ; i++)
	{
		auto btnWar = UITouchButton::create(str[i].c_str());
		btnWar->setPosition(Point(btn->getContentSize().width/2 ,btn->getContentSize().height/2)+origin);
		btnWar->setClickTarget(CC_CALLBACK_1(UIUserMinUi::onClickStaticWar,this));
		btnWar->setScale(0.1f);
		btnWar->setVisible(false);
		btnWar->setTag(i+1);
		btn->addChild(btnWar);
		m_vWarBtnUi.pushBack(btnWar);
	}
}

void UIUserMinUi::createChat()
{
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto btnChat = UITouchButton::create("btn/btn_chat.png");
	btnChat->setPosition(Point(btnChat->getContentSize().width*2 ,btnChat->getContentSize().height/2+5)+origin);
	//btnWar->setClickTarget(CC_CALLBACK_1(UIUserLayer::onClickDownUiCallbacke,this));
	this->addChild(btnChat,10);
	_btnChat = btnChat;
	auto sprite1 = Sprite::create("ui/1269.0.png");
	sprite1->setPosition(Point(-sprite1->getContentSize().width/2 ,btnChat->getContentSize().height/2) + origin);
	sprite1->setTag(1);
	sprite1->setVisible(false);
	btnChat->addChild(sprite1);

	auto label = Label::createWithSystemFont("1","",24);
	sprite1->addChild(label);
	label->setPosition(sprite1->getContentSize().width/2,sprite1->getContentSize().height/2);
	label->setTag(1);

	auto sprite2 = Sprite::create("ui/1269.0.png");
	sprite2->setPosition(Point(sprite2->getContentSize().width/2 ,btnChat->getContentSize().height/2) + origin);
	sprite2->setTag(2);
	btnChat->addChild(sprite2);
	sprite2->setVisible(false);
	auto label2 = Label::createWithSystemFont("1","",24);
	sprite2->addChild(label2);
	label2->setPosition(sprite2->getContentSize().width/2,sprite2->getContentSize().height/2);
	label2->setTag(1);

	auto node = Scale9Sprite::create("ui/chat.png");
	this->addChild(node);
	node->setPosition(Point(btnChat->getPositionX()+node->getContentSize().width/2,btnChat->getPositionY()));
	_lbChat = Label::createWithSystemFont(" ","",24);
	_lbChat->setAnchorPoint(Point(0,0.5));
	_lbChat->setPosition(55,node->getContentSize().height/2);
	node->addChild(_lbChat);
}


void UIUserMinUi::onClickDownUiCallbacke(Ref* sender)
{
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto item =  static_cast<MenuItem*>(sender);
	auto btn = item->getParent();

	if(!_bPopUp){return ;}
	_bPopUp = false;
	Point MovePos[4] = {Point(-130,-15),Point(-103,54),Point(-55,110),Point(18,143)};
	Point BackPos = Point(item->getContentSize().width/2,item->getContentSize().height/2);
	for (int i = 0; i <= 4;i++ )
	{    
		auto btnWar = item->getChildByTag(i+1);
		if(btnWar && !btnWar->isVisible())
		{
			auto Ani1 = MoveTo::create(0.25f,MovePos[i]);
			auto Ani2 = ScaleTo::create(0.25f,1.0f);
			auto spawn = Spawn::createWithTwoActions(Ani1, Ani2);
			auto EaseSine = EaseSineIn::create(spawn);
			auto callFunc = CallFunc::create(CC_CALLBACK_0(UIUserMinUi::callBackPopUpBtnOk,this));
			btnWar->runAction(Sequence::createWithTwoActions(EaseSine, callFunc));
			btnWar->setVisible(true);
		}
		else if(btnWar && btnWar->isVisible() )
		{
			Point Pos = Point(btnWar->getContentSize().width/2,btnWar->getContentSize().height/2);
			auto Ani1 = MoveTo::create(0.25f,BackPos-Pos);
			auto Ani2 = ScaleTo::create(0.25f,0.1f);
			auto spawn = Spawn::createWithTwoActions(Ani1, Ani2);
			auto EaseSine = EaseSineIn::create(spawn);
			auto callFunc = CallFunc::create(CC_CALLBACK_0(UIUserMinUi::callBackBounceBtnOk,this));
			btnWar->runAction(Sequence::createWithTwoActions(EaseSine, callFunc));
		}
	}
}

void UIUserMinUi::callBackPopUpBtnOk()
{
	_bPopUp = true;
}

void UIUserMinUi::callBackBounceBtnOk()
{
	for(auto item:m_vWarBtnUi )
	{
		item->setVisible(false);
	}
	_bPopUp = true;

}

void UIUserMinUi::setHeroAddLv(int iLv)
{
	char string[24] = {0};
	sprintf(string, "%s%d", "Lv.",iLv);
	_lbLv->setString(string);
}

void UIUserMinUi::setHeroAddExp(int nNum )
{
	if(m_HeadLayer)
		m_HeadLayer->setAddNum(nNum);
}


//玩家初始经验
void UIUserMinUi::InitHeroExp(int nCurNum,int nMaxNum)
{
	if(m_HeadLayer)
		m_HeadLayer->setinitNum(nCurNum,nMaxNum);
}
//设置玩家最大经验值
void UIUserMinUi::setHeroMaxExp(int nNum)
{
	if(m_HeadLayer)
		m_HeadLayer->setMaxNum(nNum);
}

//设置上TopUi值 @nType == 1:   ，2:   ，3:
void UIUserMinUi::addTopUiNum(int nType,int nNum)
{
	if(m_vTopUi.size() < nType ) return ;
	auto InfoLayer = m_vTopUi.at(nType-1);
	if(InfoLayer)
		InfoLayer->setAddNum(nNum);
}

//初始化TopUi 当前值 上限值 名字
void UIUserMinUi::InitTopUiNum(int nType,int nCurNum, int nMaxNum, const string& text)
{
	if(m_vTopUi.size() < nType ) return ;
	auto InfoLayer = m_vTopUi.at(nType-1);
	if(InfoLayer)
		InfoLayer->setinitNum(nCurNum,nMaxNum,text);
}
//TopUi 设置最大值
void UIUserMinUi::setTopUiMaxNum(int nType,int nMaxNum)
{
	if(m_vTopUi.size() < nType ) return ;
	auto InfoLayer = m_vTopUi.at(nType-1);
	if(InfoLayer)
		InfoLayer->setMaxNum(nMaxNum);
}

void UIUserMinUi::addRightUiNum(int nType,int nNum)
{
	if(m_vRightUi.size() < nType ) return ;
	auto InfoLayer = m_vRightUi.at(nType-1);
	if(InfoLayer)
		InfoLayer->setAddNum(nNum);
}

//初始化RightUi 当前值 上限值 名字
void UIUserMinUi::InitRightUiNum(int nType,int nCurNum, int nMaxNum, const string& text)
{
	if(m_vRightUi.size() < nType ) return ;
	auto InfoLayer = m_vRightUi.at(nType-1);
	if(InfoLayer)
		InfoLayer->setinitNum(nCurNum,nMaxNum,text);
		
}
//RightUi 设置最大值
void UIUserMinUi::setRightUiMaxNum(int nType,int nMaxNum)
{
	if(m_vRightUi.size() < nType ) return ;
	auto InfoLayer = m_vRightUi.at(nType-1);
	if(InfoLayer)
		InfoLayer->setMaxNum(nMaxNum);
}

void UIUserMinUi::setChatString(const string& text)
{
	_lbChat->setString(text);
}

void UIUserMinUi::setChatTip(int nType,int Num)
{
	if(_btnChat->getChildByTag(nType) && Num >0)
	{
		_btnChat->getChildByTag(nType)->setVisible(true);
		auto label = (Label*)_btnChat->getChildByTag(nType)->getChildByTag(1);
		label->setString(String::createWithFormat("%d",Num)->getCString());
	}
}

void UIUserMinUi::onClickStaticWar( Ref* sender )
{
	if(_btnStaticWarCallBack)
	{
		_btnStaticWarCallBack(sender);
	}
}

void UIUserMinUi::setStaticWarCallBack(const ccMenuCallback& callback )
{
	_btnStaticWarCallBack = callback;
}

