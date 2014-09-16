#include "UIHeroConfiguration.h"
#include "HeroConfig.h"

UIHeroConfiguration::UIHeroConfiguration()
{
	m_CurStateIndex = 0;
	_nHeroBattleId = 0;
	_HeroinfoNode = nullptr;
}
UIHeroConfiguration::~UIHeroConfiguration()
{

}

bool UIHeroConfiguration::init()
{
	if (UIMsgLayer::init())
	{   
		initWithForm();
		return true;
	}
	return false;
}

void UIHeroConfiguration::initWithForm()
{

	//下边框 滑动
	auto tmpbtn = UITouchButton::create(String::createWithFormat("%s","btn/btn_hero_head01.png")->getCString());
	auto bg = Scale9Sprite::create("dian9/huangban.9@2x.png");
	bg->setContentSize(Size(this->getContentInfoNode()->getContentSize().width*0.98,tmpbtn->getContentSize().height*1.4));
	this->getContentInfoNode()->addChild(bg);
	bg->setAnchorPoint(Point(0,0));
	bg->setPosition(Point(0,15));
	auto ContentNode = bg;

	auto Nodesize = bg->getContentSize();
	ScrollView * scrollView = ScrollView::create();
	scrollView->setViewSize(Nodesize);
	scrollView->setPosition(Point(0,0));
	ContentNode->addChild(scrollView);
	scrollView->setTouchEnabled(true);
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL);

	auto ve = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevel();
	scrollView->setContentSize(Size(Nodesize.width/5*ve.size(),Nodesize.height));
	/*TableViewDataSource* data;
	TableView::create(data,Size(50,50));*/
	auto Path = String::createWithFormat("hero/hero%02d_head.png",1)->getCString();
	auto strHeadPath = String::createWithFormat("%s","btn/btn_hero_head01.png")->getCString();
	int iNum = 0;
	for(auto item:ve)
	{
		auto info = item.second;
		auto btn = UITouchButton::create(strHeadPath);
		btn->setPosition(Nodesize.width/5 *iNum +btn->getContentSize().width/2+15,Nodesize.height/2);
		btn->setIsUsedForScroller(true);
		btn->setClickTarget(CC_CALLBACK_1(UIHeroConfiguration::onClickTableBtnCallBack,this));
		//背景
		auto spBg = Sprite::create(String::createWithFormat("%s","btn/btn_hero_head03.png")->getCString());
		btn->addChild(spBg,0,1);
		spBg->setVisible(false);

		auto spLogo = Sprite::create(String::createWithFormat("%s","ui/yes_02.png")->getCString());
		btn->addChild(spLogo,10,5);
		spLogo->setPosition(-btn->getContentSize().width/2+spLogo->getContentSize().width/2,btn->getContentSize().height/2-spLogo->getContentSize().height/2);
		spLogo->setVisible(false);

		//头像
		auto sp = Sprite::create(Path);
		btn->addChild(sp);

		scrollView->addChild(btn,10,info->id);
		iNum++;
		m_TableBtn.pushBack(btn);
	}


}

void UIHeroConfiguration::tabsFunc( int nHeroId )
{
	if(m_CurStateIndex == nHeroId) return ;
	m_CurStateIndex = nHeroId;
	for(auto itme:m_TableBtn)
	{
		if(itme->getTag() == nHeroId)
		{
			itme->getChildByTag(1)->setVisible(true);
		}
		else 
		{
			itme->getChildByTag(1)->setVisible(false);
		}
	}

	createHeroInfoLayer(nHeroId);
}

void UIHeroConfiguration::onClickTableBtnCallBack( Ref* sender )
{
	auto tag =  static_cast<MenuItem*>(sender)->getTag();
	tabsFunc(tag);
}

void UIHeroConfiguration::createHeroInfoLayer( int nHeroId )
{
	if(getHeroinfoNode())
	{
		getHeroinfoNode()->removeAllChildren();
	}
	else
	{
		_HeroinfoNode = Node::create();
		auto size = UITouchButton::create(String::createWithFormat("%s","btn/btn_hero_head01.png")->getCString())->getContentSize();
		_HeroinfoNode->setContentSize(Size(this->getContentInfoNode()->getContentSize().width,this->getContentInfoNode()->getContentSize().height - size.height*1.4));
		_HeroinfoNode->setPosition(0,size.height*1.4);
		_HeroinfoNode->setAnchorPoint(Point(0,0));
		this->getContentInfoNode()->addChild(_HeroinfoNode);
	}
	auto ve = HeroInfo::getHeroConfig();
	auto it = ve.find(nHeroId);
	if(it != ve.end())
	{
		auto info = it->second;
		//头像
		auto strFormPath = String::createWithFormat("%s","form/form004.png")->getCString();
		auto btnHerd = UITouchButton::create(strFormPath);
		_HeroinfoNode->addChild(btnHerd,10,info->id);
		btnHerd->setPosition(btnHerd->getContentSize().width/2,_HeroinfoNode->getContentSize().height/2);
		btnHerd->setClickTarget(CC_CALLBACK_1(UIHeroConfiguration::onClickShowHeroTip,this));

		auto Path = String::createWithFormat("hero/hero%02d.png",1);
		auto spHrad = Sprite::create(Path->getCString());
		btnHerd->addChild(spHrad);
		spHrad->setScale(btnHerd->getContentSize().width/spHrad->getContentSize().width-0.1f);
		//spHrad->setPosition(btnHerd->getContentSize().width/2,btnHerd->getContentSize().height/2);
		auto UpDataInfo = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(info->id);
		if(!UpDataInfo) return;
		//名字
		auto str = String::createWithFormat("%s  Lv.%d",info->name.c_str(),UpDataInfo->herolevel)->getCString();
		auto lable = Label::createWithSystemFont(str,"",32);
		lable->setAnchorPoint(Point(0,0.5));
		_HeroinfoNode->addChild(lable);
		lable->setPosition(btnHerd->getContentSize().width*1.1,_HeroinfoNode->getContentSize().height - lable->getContentSize().height);
		lable->setColor(Color3B::BLACK);

		//属性
		auto HeroData = HeroUpgrade::getHeroUpgrageFromId(info->id*100+UpDataInfo->herolevel);
		vector<string> mTmpStr;
		mTmpStr.push_back(String::createWithFormat("attack speed: %d",info->attackspeed)->getCString());
		mTmpStr.push_back(String::createWithFormat("attack : %d",HeroData->hurt)->getCString());
		mTmpStr.push_back(String::createWithFormat("hp : %d",HeroData->hp)->getCString());
		mTmpStr.push_back(String::createWithFormat("attack AOE : %d",info->AOErange)->getCString());

		int iNum = 0;
		for(auto item:mTmpStr)
		{
			auto addlable = Label::createWithSystemFont(item,"",24);
			_HeroinfoNode->addChild(addlable);
			addlable->setAnchorPoint(Point(0,0.5));
			addlable->setPosition(btnHerd->getContentSize().width*1.1,lable->getPositionY()-lable->getContentSize().height*2-addlable->getContentSize().height*iNum);
			addlable->setColor(Color3B::BLACK);
			iNum++;
		}
		mTmpStr.clear();

		//技能
		auto spSkill = Sprite::create("ui/700010.png");
		spSkill->setAnchorPoint(Point(1,0.5));
		_HeroinfoNode->addChild(spSkill);
		spSkill->setPosition(_HeroinfoNode->getContentSize().width*0.95,_HeroinfoNode->getContentSize().height/2);

		auto lbSkill = Label::createWithSystemFont("Skill","",24);
		spSkill->addChild(lbSkill);
		lbSkill->setAnchorPoint(Point(0.5,0));
		lbSkill->setColor(Color3B::BLACK);
		lbSkill->setPosition(spSkill->getContentSize().width*0.5,spSkill->getContentSize().height);

		//出征
		auto btnBattle = UITouchButton::create("btn/btn_jiasu.png");
		_HeroinfoNode->addChild(btnBattle,10,info->id*10);
		btnBattle->setPosition(_HeroinfoNode->getContentSize().width - btnBattle->getContentSize().width/1.8,btnBattle->getContentSize().height/1.2);
		btnBattle->setClickTarget(CC_CALLBACK_1(UIHeroConfiguration::onClickBattleOrOut,this));


		auto Altarinfo = HeroAltarConfig::getHeroAltarConfigFromId(_db->id);
		if(Altarinfo)
		{
			for(auto itme:m_TableBtn)
			{
				if(itme->getTag() == Altarinfo->type)
				{
					_nHeroBattleId = Altarinfo->type;
					itme->getChildByTag(5)->setVisible(true);
					break;
				}
			}
		}


		str = String::createWithFormat("%s","Battle")->getCString();
		if(info->id == _nHeroBattleId)
		{
			str = String::createWithFormat("%s","out")->getCString();
		}
		auto lbBattle = Label::createWithSystemFont(str,"",24);
		btnBattle->addChild(lbBattle,10,1);
	}
}

void UIHeroConfiguration::onClickShowHeroTip( Ref* sender )
{
	auto tag =  static_cast<MenuItem*>(sender)->getTag();
	auto tipLayer = HeroTip::showHeroTip(tag);
	addChild(tipLayer->getTipLayer());


}

void UIHeroConfiguration::onClickBattleOrOut( Ref* sender )
{
	auto btn = static_cast<MenuItem*>(sender);
	auto tag =  btn->getTag()/10;
	for(auto itme:m_TableBtn)
	{
		if(itme->getTag() == tag && _db)
		{
			bool boolen = true;
			if(tag == _nHeroBattleId && itme->getChildByTag(5)->isVisible()){
				boolen = false;
				static_cast<Label*>(btn->getChildByTag(1))->setString("Battle");
				auto info = HeroAltarConfig::getHeroAltarConfigFromId(_db->id);
				if(info)
				{
					info->UpdataHeroToAltar(0);
				}
			}
			else
			{
				static_cast<Label*>(btn->getChildByTag(1))->setString("out");
				auto info = HeroAltarConfig::getHeroAltarConfigFromId(_db->id);
				if(info)
				{
					info->UpdataHeroToAltar(itme->getTag());
				}
				else 
					HeroAltarConfig::createHeroToAltar(_db->id,itme->getTag());
			}
			itme->getChildByTag(5)->setVisible(boolen);
		}
		else 
		{
			itme->getChildByTag(5)->setVisible(false);
		}
	}
	_nHeroBattleId = tag;

}

UIHeroConfiguration* UIHeroConfiguration::CreateFromDB( DBuilding * db )
{
	auto item = UIHeroConfiguration::create();
	item->_db = db;
	auto info = HeroAltarConfig::getHeroAltarConfigFromId(db->id);
	if(info){
		item->tabsFunc(info->type);
	}
	else {
		HeroAltarConfig::createHeroToAltar(db->id,0);
	}
	item->UpDataTableBtn();
	return item;
}

void UIHeroConfiguration::UpDataTableBtn()
{
	if(_db){
		auto info = HeroAltarConfig::getHeroAltarConfig();
		for (auto item:info)
		{
			auto heroAltar = item.second;
			for (auto btnItem:m_TableBtn)
			{
				if(btnItem->getTag() == heroAltar->type)
				{
					if(_db->id == heroAltar->id)
					{
						break;
					}
					btnItem->setEnabled(false);
					auto lb = Label::createWithSystemFont("XXXX","",24);
					lb->setColor(Color3B::ORANGE);
					btnItem->addChild(lb);
					break;
				}
			}

		}
	}
}


