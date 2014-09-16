#include "UIHeroPub.h"
#include "AppMacros.h"
#define CON Configuration::getInstance()
#include "BuildingConfig.h"
#include "HeroConfig.h"

UIHeroPub::UIHeroPub()
{
	m_CurStateIndex = 0;
	_CruClickId = 0;
	_CurLvUpHeroId = 0;
	_CruClickTipId = 0;
	m_RecruitHeroNode = nullptr;
	m_HeroListNode = nullptr;
	m_HeroUpLvLabel = nullptr;
	m_SkillUpLvLabel = nullptr;
	m_HeroUpLvBtn = nullptr;
	m_SkillUpLvBtn = nullptr;
	_scale9Sprite = nullptr;
	_db = nullptr;
	m_TipLayer = nullptr;

}
UIHeroPub::~UIHeroPub()
{
	m_CurStateIndex = 0;
	_CruClickId = 0;
	_CurLvUpHeroId = 0;
	_CruClickTipId = 0;
}

 bool UIHeroPub::init()
 {
	 if (UIMsgLayer::init())
	{   
		this->setVIsibelLeftRightBtn(false);
		initWithForm();
		this->schedule(schedule_selector(UIHeroPub::updateWorkProcess),1.0f);
		return true;
	}
	 return false;
 }

 void UIHeroPub::initWithForm()
 {
	m_BtnName.push_back("RecruitHero");
	m_BtnName.push_back("HeroList");
	createTableBtn(m_BtnName.size());

	//tabsFunc(1);
 }

void UIHeroPub::createTableBtn(int MaxNum)
{
	auto Tmpbtn = UITouchButton::create("ui/normal.png");
	auto Title = getConTentNode();
	auto node9 = Scale9Sprite::create("form/form003.png"); 
	Title->addChild(node9,0);
	node9->setAnchorPoint(Point(0.5,0));
	node9->setPosition(Title->getContentSize().width/2,28);
	_scale9Sprite= node9;
	for(int i=1;i <= MaxNum;i++){
		auto btn = UITouchButton::create("btn/btn_tabel_nor.png");
		Title->addChild(btn,10+i,i);
		btn->setPosition(btn->getContentSize().width/2+(i-1)*btn->getContentSize().width,node9->getContentSize().height+node9->getPositionY()+btn->getContentSize().height/2-1);
		btn->setClickTarget(CC_CALLBACK_1(UIHeroPub::onClickTableBtnCallBack,this));

		auto sp = Sprite::create("btn/btn_tabel_sel.png");
		btn->addChild(sp,9,1);
		sp->setVisible(false);
		auto label = Label::createWithSystemFont(m_BtnName.at(i-1),"",20);
		btn->addChild(label,10);
		m_TableBtn.pushBack(btn);
	}

}

void UIHeroPub::onClickTableBtnCallBack(Ref* sender)
{
	auto tag =  static_cast<MenuItem*>(sender)->getTag();
	tabsFunc(tag);

}

void UIHeroPub::tabsFunc(int index)
{
	if(m_CurStateIndex == index) return ;
	m_CurStateIndex = index;
	_CruClickId = 0;
	for(auto itme:m_TableBtn)
	{
		if(itme->getTag() == index)
		{
			itme->getChildByTag(1)->setVisible(true);
		}
		else 
			itme->getChildByTag(1)->setVisible(false);
	}
	if(index == 1)
	{
		if(m_HeroListNode){
			m_HeroListNode->setVisible(false);
		}

		createRecruitHero();
	}
	else if(index == 2)
	{
		if(m_RecruitHeroNode){
			m_RecruitHeroNode->setVisible(false);
		}
		createHeroList();
	}
}


void UIHeroPub::createRecruitHero()
{
	if(m_RecruitHeroNode){
		m_RecruitHeroNode->setVisible(true);
		m_RecruitHeroNode->removeFromParent();
	}

	auto Nodesize = _scale9Sprite->getContentSize();
	ScrollView * scrollView = ScrollView::create();
	scrollView->setViewSize(Nodesize);
	scrollView->setPosition(Point(0,0));
	_scale9Sprite->addChild(scrollView);
	scrollView->setTouchEnabled(true);
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
	m_RecruitHeroNode = scrollView;


	auto node = Node::create();
	node->setContentSize(Size(Nodesize.width,Nodesize.height));
	scrollView->getContainer()->addChild(node);
	int size = 0;
	auto ve = HeroInfo::getHeroConfig();
	if((int)ve.size()%3 != 0)
	{
		size = 1;
	}
	scrollView->setContentSize(Size(Nodesize.width*((int)ve.size()/3+size),Nodesize.height));//拖拽长度

	int index = 0;	//当前数量
	int Page = 0;	//当前页数
	for (auto item : ve)
	{
		if(index%3 == 0 )
		{
			Page++;
		}
		auto info = item.second;
		auto layer = UIRecruitHeroHerdLayer::create(info->id,UIRecruitHeroHerdLayer::RecruitType::BIG);
		node->addChild(layer,0,info->id);
		float posx = ((index%3)+((Page-1)*3))*node->getContentSize().width/3+12;
		layer->setPosition(posx,node->getContentSize().height/2-layer->getContentSize().height/2);
		layer->setCallBack(CC_CALLBACK_1(UIHeroPub::onClickRecruit,this));
		layer->setDownName(String::createWithFormat("dd==%d==dd",index)->getCString());
		index++;
		auto heroInfo = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(info->id);
		if(heroInfo)
		{
			layer->getHeadbtn()->setEnabled(false);
			layer->setDownName(String::createWithFormat("%s","Already have")->getCString());
			layer->getDownName()->setColor(Color3B::BLACK);
		}
	}

}
void UIHeroPub::createHeroList()
{
	if(m_HeroListNode){
		m_HeroListNode->setVisible(true);
		m_HeroListNode->removeFromParent();
	}
	auto Nodesize = _scale9Sprite->getContentSize();
	ScrollView * scrollView = ScrollView::create();
	scrollView->setViewSize(Nodesize);
	scrollView->setPosition(Point(0,0));
	_scale9Sprite->addChild(scrollView);
	scrollView->setTouchEnabled(true);
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
	m_HeroListNode = scrollView;

	auto node  = Node::create();
	node->setContentSize(Nodesize);
	m_HeroListNode->getContainer()->addChild(node);
	float Width = 0;
	float Height = 0;

	auto ve = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevel();
		
	int inum = 0;
	if(ve.size()%8 != 0)
	{
		inum = 1;
	}
	if(ve.size()/8+inum <= 1)
	{
		scrollView->setTouchEnabled(false);
	}
	scrollView->setContentSize(Size(Nodesize.width*(ve.size()/8+inum),Nodesize.height));
	int i = 0;
	int Page = 0;
	for(auto item : ve){
		int index = i-(Page)*8;
		auto info = item.second;
		auto layer2 = UIRecruitHeroHerdLayer::create(info->id,UIRecruitHeroHerdLayer::RecruitType::SMALL);
		node->addChild(layer2);
		if(index < 4)
		{
			Width = (index+(Page)*4)*(node->getContentSize().width/4)+layer2->getContentSize().width/3;
			Height = node->getContentSize().height/1.5 - layer2->getContentSize().height/4;
		}
		else
		{
			int page = index-4;
			Width = (page+(Page)*4)*(node->getContentSize().width/4)+layer2->getContentSize().width/3;
			Height = node->getContentSize().height/3 - layer2->getContentSize().height/1.3;
		}
		layer2->setPosition(Width,Height);
		layer2->setTopName(HeroInfo::getHeroFromId(info->id)->name.c_str());
		layer2->setCallBack(CC_CALLBACK_1(UIHeroPub::onClickHeroTip,this));
		layer2->setTag(info->id);


		auto spLvBg = Sprite::create("ui/mask_01.png");
		layer2->getHeadbtn()->addChild(spLvBg,9);
		spLvBg->setPosition(0,-layer2->getContentSize().height/2 + spLvBg->getContentSize().height/2);
		int nLv = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(info->id)->herolevel;
		auto label = Label::createWithSystemFont(String::createWithFormat("Lv.%d",nLv)->getCString(),"",24);
		label->setPosition(0,spLvBg->getPositionY());
		label->setColor(Color3B::GREEN);
		layer2->getHeadbtn()->addChild(label,10,100);
		auto UpInfo =HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(info->id);
		if(UpInfo->status == 1)
		{
			layer2->setDownName(String::createWithFormat("%d",UpInfo->getTotalTime())->getCString());
			_CurLvUpHeroId = info->id;
		}
		m_HeroHerdLayer.pushBack(layer2);

		i++;
		if(i%8 == 0){Page++;}

	}


}

void UIHeroPub::onClickRecruit(Ref* sender)
{
	if(m_CurStateIndex != (int)PubStatus::HERO_RECRUIT) return ;
	auto item =  static_cast<UIRecruitHeroHerdLayer*>(sender);
	/*if(_CruClickId == item->getTag()) return ;*/
	_CruClickId = item->getTag();
	HeroLevelAndSkillLevel::createHeroItem(_CruClickId);
	item->getHeadbtn()->setEnabled(false);
	item->setDownName(String::createWithFormat("%s","Already have")->getCString());
	item->getDownName()->setColor(Color3B::BLACK);
}

void UIHeroPub::onClickHeroTip(Ref* sender)
{
	if(m_CurStateIndex != (int)PubStatus::HERO_LIST) return ;
	auto item =  static_cast<UIRecruitHeroHerdLayer*>(sender);

	createHeroTipLayer(item->getHeroInfo()->id);
}


void UIHeroPub::createHeroTipLayer( int nHeroId )
{
	auto info = HeroInfo::getHeroFromId(nHeroId);
	auto UpInfo =HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(info->id);
	_CruClickTipId = nHeroId;
	if(m_TipLayer)
	{
		m_TipLayer->UpDataHeroTip(info);
	}
	else 
	{
		auto tip = HeroTip::showHeroTip(info->id);
		addChild(tip->getTipLayer());
		m_TipLayer = tip;
	}
	auto tip = m_TipLayer->getTipLayer();
	int nHeroLv = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(info->id)->herolevel;
	if(nHeroLv == 0 ) nHeroLv = 1;
	auto HeroData = HeroUpgrade::getHeroUpgrageFromId(info->id*100+nHeroLv);
	auto TmpHeroData = HeroUpgrade::getHeroUpgrageFromId(info->id*100+nHeroLv+1);
	//升级增加指数
	if(TmpHeroData){
		int nAddNum = TmpHeroData->hurt - HeroData->hurt;
		auto str = String::createWithFormat("+%d",nAddNum)->getCString();
		tip->setTopBegin1String(UITipLayer::TopBegin::TopBeginNode2,1,str,1,Color3B::RED);

		nAddNum = TmpHeroData->hp - HeroData->hp;
		str = String::createWithFormat("+%d",nAddNum)->getCString();
		tip->setTopBegin1String(UITipLayer::TopBegin::TopBeginNode2,2,str,1,Color3B::RED);
	}
	tip->setColseCallBack(CC_CALLBACK_0(UIHeroPub::onCloseTip,this));
	auto size =Size(tip->getContentInfoNode()->getContentSize().width/2,tip->getContentInfoNode()->getContentSize().height*0.98);
	auto Pos = Point(tip->getContentInfoNode()->getContentSize().width/2,5);
	//人物升级按钮
	auto btn = UITouchButton::create("btn/btn_jiasu.png");
	btn->setPosition(size.width/2,Pos.y+btn->getContentSize().height/1.5);
	tip->getContentInfoNode()->addChild(btn,10,info->id*10);
	btn->setClickTarget(CC_CALLBACK_1(UIHeroPub::onClickUpHeroLv,this));
	m_HeroUpLvBtn = btn;
	auto spMoney = Sprite::create("ui/73.0.png");			//图
	auto lbMoney = Label::createWithSystemFont(" ","",24);	//数值
	m_HeroUpLvBtn->addChild(spMoney);
	m_HeroUpLvBtn->addChild(lbMoney);
	create2NodeMiddle(m_HeroUpLvBtn,lbMoney,spMoney);
	//按钮标题
	m_HeroUpLvLabel = Label::createWithSystemFont(" ","",24); 
	m_HeroUpLvLabel->setColor(Color3B::RED);
	m_HeroUpLvLabel->setAnchorPoint(Point(0.5f,0));
	m_HeroUpLvLabel->setPosition(btn->getPositionX(),btn->getPositionY()+btn->getContentSize().height/2);
	tip->getContentInfoNode()->addChild(m_HeroUpLvLabel,10);
	if(UpInfo->status == 1 && UpInfo->isupgradehero == 0)
	{
		m_HeroUpLvLabel->setString(String::createWithFormat("%d",UpInfo->timestamp)->getCString());
	}
	//升级消耗
	int nLv = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(info->id)->herolevel;
	if(nLv == 0 ) nLv =1;
	auto HeroBasedata = HeroBaseUpgrade::getHeroBaseFromId(nLv);
	HeroUpDataMoney(String::createWithFormat("%d",HeroBasedata->cost)->getCString(),"XXX");

	//技能升级按钮
	auto btn2 = UITouchButton::create("btn/btn_jiasu.png");
	btn2->setPosition(Pos.x+size.width/2,Pos.y+btn->getContentSize().height/1.5);
	tip->getContentInfoNode()->addChild(btn2,50,info->id*20);
	btn2->setClickTarget(CC_CALLBACK_1(UIHeroPub::onClickUpSkillLv,this));
	m_SkillUpLvBtn = btn2;
	auto spMoney2 = Sprite::create("ui/73.0.png");			//图
	auto lbMoney2 = Label::createWithSystemFont(" ","",24);	//数值
	m_SkillUpLvBtn->addChild(spMoney2);
	m_SkillUpLvBtn->addChild(lbMoney2);
	create2NodeMiddle(m_SkillUpLvBtn,lbMoney2,spMoney2);
	//按钮标题 
	m_SkillUpLvLabel = Label::createWithSystemFont(" ","",24); 
	m_SkillUpLvLabel->setColor(Color3B::RED);
	m_SkillUpLvLabel->setAnchorPoint(Point(0.5f,0));
	m_SkillUpLvLabel->setPosition(btn2->getPositionX(),btn2->getPositionY()+btn2->getContentSize().height/2);
	tip->getContentInfoNode()->addChild(m_SkillUpLvLabel,10);
	if(UpInfo->status == 1 && UpInfo->isupgradehero == 1)
	{
		m_SkillUpLvLabel->setString(String::createWithFormat("%d",UpInfo->timestamp)->getCString());
	}
	//技能升级消耗
	int nSkillLv = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(info->id)->skilllevel;
	auto  HeroSkilldata =  HeroSkillUpgrade::getHeroSkillFromId(info->skillid*100+nSkillLv);
	if(HeroSkilldata){
		SkillUpDataMoney(String::createWithFormat("%d",HeroSkilldata->cost)->getCString(),"XXXX");
	}

}

void UIHeroPub::create2NodeMiddle(Node* parent,Node* n1,Node* n2)
{
	auto size = parent->getContentSize();
	n1->setPosition(-n2->getContentSize().width/2,0);
	n1->setTag(1);
	n2->setPosition(n1->getPositionX()+n1->getContentSize().width/2+n2->getContentSize().width/2+5,n1->getPositionY());
	n2->setTag(2);
}
//Tip按钮
void UIHeroPub::HeroUpDataMoney(const string str,string str2,string Path)
{
	if(m_HeroUpLvBtn){
		auto lbMoney = (Label*)m_HeroUpLvBtn->getChildByTag(1);//
		auto sp = (Sprite*)m_HeroUpLvBtn->getChildByTag(2);
		lbMoney->setString(str);
		if(str2 != " ")
		{
			m_HeroUpLvLabel->setString(str2);
		}
		if(Path != " ")
		{
			sp->setTexture(Path);
		}
		create2NodeMiddle(m_HeroUpLvBtn,lbMoney,sp);
	}
}
void UIHeroPub::SkillUpDataMoney(const string str,string str2,string Path)
{
	if(m_SkillUpLvBtn){
		auto lbMoney = (Label*)m_SkillUpLvBtn->getChildByTag(1);//
		auto sp = (Sprite*)m_SkillUpLvBtn->getChildByTag(2);
		lbMoney->setString(str);
		if(str2 != " ")
		{
			m_SkillUpLvLabel->setString(str2);
		}
		if(Path != " ")
		{
			sp->setTexture(Path);
		}
		create2NodeMiddle(m_HeroUpLvBtn,lbMoney,sp);
	}
}

UIHeroPub * UIHeroPub::CreateFromDB( DBuilding * db )
{
	auto ret = UIHeroPub::create();
	ret->_db = db;
	ret->initTitle();
	ret->tabsFunc(1);
	ret->schedule(schedule_selector(UIHeroPub::updateWorkProcess),1.0f);
	return ret;
}

void UIHeroPub::initTitle()
{
	auto cfg = _db->getCurBindCfg();
	auto str = String::createWithFormat(CON->getValue("des").asString().c_str(),cfg->name.c_str(),cfg->id % 100);
	this->setTitle(str->getCString());
}

void UIHeroPub::updateWorkProcess( float dt )
{
	for (auto item:m_HeroHerdLayer)
	{
		if(item->getTag() == _CurLvUpHeroId)
		{
			auto UpInfo = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(_CurLvUpHeroId);
			auto nTime = UpInfo->getTotalTime();
			
			if(nTime > 0  && UpInfo->status == 1 )
			{
				//设置界面更新
				item->getDownName()->setString(String::createWithFormat("%d",nTime)->getCString());

				//设置打开的Tip界面 更新
				if(UpInfo->isupgradehero == 0 && _CruClickTipId == _CurLvUpHeroId)
				{
					HeroUpDataMoney("100",String::createWithFormat("%d",nTime)->getCString());
				}
				else if(UpInfo->isupgradehero == 1 && _CruClickTipId == _CurLvUpHeroId)
				{
					SkillUpDataMoney("100",String::createWithFormat("%d",nTime)->getCString());
				}
			}
			else if(nTime <=0)
			{
				if (m_TipLayer && _CruClickTipId == _CurLvUpHeroId)
				{
					createHeroTipLayer(UpInfo->id);
				}
				auto lb = (Label*)item->getHeadbtn()->getChildByTag(100);
				int nLv = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(UpInfo->id)->herolevel;
				lb->setString(String::createWithFormat("Lv.%d",nLv)->getCString());
				item->getDownName()->setString(" ");

				_CurLvUpHeroId = 0;
			}
		}
	}

}

void UIHeroPub::onClickUpHeroLv( Ref* sender )
{
	auto Tag =  static_cast<UITouchButton*>(sender)->getTag()/10;
	auto Info = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(Tag);
	if(Info->setUpData(0))//英雄升级
	{
		_CurLvUpHeroId = Tag;
	}
}

void UIHeroPub::onClickUpSkillLv( Ref* sender )
{
	auto Tag =  static_cast<UITouchButton*>(sender)->getTag()/20; 
	auto Info = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(Tag);
	if(Info->setUpData(1))//英雄技能升级
	{
		_CurLvUpHeroId = Tag;
	}
}

void UIHeroPub::onCloseTip()
{
	m_HeroUpLvBtn = nullptr;
	m_SkillUpLvBtn = nullptr;
	m_HeroUpLvLabel = nullptr;
	m_SkillUpLvLabel = nullptr;
	m_TipLayer = nullptr;
}