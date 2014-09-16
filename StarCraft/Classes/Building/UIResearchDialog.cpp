#include "UIResearchDialog.h"
#include "BuildingConfig.h"
#include "SoldierConfig.h"
#include "UITouchButton.h"
#include "UISoldierItem.h"

#include "extensions/cocos-ext.h"
using namespace cocos2d::extension;
#define CON Configuration::getInstance()

#include "BarrackData.h"
#include "UITipLayer.h"

UIResearchDialog::UIResearchDialog()
{
    _db = nullptr;
	_tiplayer = nullptr;
	_selectItem = nullptr;
}

UIResearchDialog * UIResearchDialog::CreateFromDB( DBuilding * db )
{
	auto item = UIResearchDialog::create();
	item->_db = db;

	item->getContextNode()->setContentSize(Size(650,520));
	auto cfg = db->getCurBindCfg();
	auto str = String::createWithFormat(CON->getValue("des").asString().c_str(),cfg->name.c_str(),cfg->id % 100);
	item->setTitle(str->getCString());

	item->initUpgradeSoldier();
	item->initSoldierItems();

	return item;
}

void UIResearchDialog::initUpgradeSoldier(bool isfirst)
{   
    
    auto cfg = SoldierUpgradeData::getSoldierUpgradeMapFromId(_db->id);
	if (cfg == nullptr || cfg->getSoldierLevel() == nullptr)
	{
		return;
	}
	auto soldiercfg = cfg->getSoldierLevel();
	auto picstr = String::createWithFormat("ui/%d.png",cfg->type);
	setTimerNode(Node::create());
	setInfoPic(Sprite::create(picstr->getCString()));

	
    auto str1   = String::createWithFormat(CON->getValue("soldierlabel").asString().c_str(),soldiercfg->getCurcfg()->name.c_str(),soldiercfg->getCurcfg()->id % 100,soldiercfg->getNextcfg()->id % 100);
	auto label1 = Label::createWithSystemFont(str1->getCString(),"",20);
	label1->setColor(Color3B::BLACK);
	label1->setPositionY(50);
	getTimerNode()->addChild(label1);
   
	auto str2   = String::createWithFormat(CON->getValue("attackadd").asString().c_str(),soldiercfg->getCurcfg()->attack,soldiercfg->getNextcfg()->attack - soldiercfg->getCurcfg()->attack );
	auto label2 = Label::createWithSystemFont(str2->getCString(),"",20);
	label2->setColor(Color3B::BLACK);
	label2->setPositionY(25);
	getTimerNode()->addChild(label2);

	auto str3   = String::createWithFormat(CON->getValue("hpadd").asString().c_str(),soldiercfg->getCurcfg()->max_hp,soldiercfg->getNextcfg()->max_hp - soldiercfg->getCurcfg()->max_hp);
	auto label3 = Label::createWithSystemFont(str3->getCString(),"",20);
	label3->setColor(Color3B::BLACK);
	label3->setPositionY(0);
	getTimerNode()->addChild(label3);
     

	auto str4 = String::createWithFormat(CON->getValue("traintime").asString().c_str(),cfg->getLeftTime());
	auto label4 = Label::createWithSystemFont(str4->getCString(),"",20);
	label4->setColor(Color3B::BLACK);
	label4->setPosition(Point(200,50));
	label4->setTag(111);
	getTimerNode()->addChild(label4);
	this->schedule(schedule_selector(UIResearchDialog::updateTimer),1.0f);

	auto btn = UITouchButton::create("ui/button.png");
	btn->setPosition(Point(200,-20));
	auto str5    = String::createWithFormat("%d",soldiercfg->getNextcfg()->upgrade_cost);
	auto label5  = Label::createWithSystemFont(str5->getCString(),"",20);
    btn->addChild(label5);
	getTimerNode()->addChild(btn);

	if (!isfirst)
	{   
		auto pCenter = Point(getContextNode()->getContentSize().width / 2 ,getContextNode()->getContentSize().height / 2 );
		Size contentSize  = getContextNode()->getContentSize();
		if (getTimerNode() != nullptr)
		{   
			//TODO 设置位置
			getTimerNode()->setPosition(pCenter + Point(30.0f, contentSize.height / 2 - 150.f));
			getContextNode()->addChild(getTimerNode());
		}
		if (getInfoPic() != nullptr)
		{   
			getInfoPic()->setAnchorPoint(Point(0,1));
			getInfoPic()->setPosition(pCenter + Point( - contentSize.width / 2  + 30.0f, contentSize.height / 2 - 100.f));
			getContextNode()->addChild(getInfoPic());
		}
	}

}

void UIResearchDialog::initSoldierItems()
{

	//TODO 创建一个Layer 放士兵按钮

	auto layer = UISoldierUpgradeLayer::create();
	layer->setCallback(CC_CALLBACK_1(UIResearchDialog::onSolderItemClick,this));

	auto scrollview = ScrollView::create();
	scrollview->setPosition(Point(20,50));
	scrollview->setContentOffset(Point::ZERO);
	scrollview->setViewSize(Size(getContextNode()->getContentSize().width - 40,240));
	scrollview->setDirection(ScrollView::Direction::HORIZONTAL);
    scrollview->setContainer(layer);
	getContextNode()->addChild(scrollview,100);

}

void UIResearchDialog::onSolderItemClick( Ref* ref )
{      
	auto item = dynamic_cast<UISoldierItem*>(ref);
	_selectItem = item;
	auto soldiercfg = item->getData();

    auto tip = UITipLayer::create();
	_tiplayer = tip;
	

	//标题
	auto strtitle = String::createWithFormat(CON->getValue("des").asString().c_str(),soldiercfg->getCurcfg()->name.c_str(),soldiercfg->getCurcfg()->id % 100);
	tip->setTitle(strtitle->getCString());
    
	//图片
	auto img = Sprite::create(String::createWithFormat("ui/%d.png",soldiercfg->getCurcfg()->id /100)->getCString());
	tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode3,img,Point(5,tip->getContentInfoNode()->getContentSize().height / 2));

	auto str = String::createWithFormat(CON->getValue("upgradetime").asString().c_str(),soldiercfg->getNextcfg()->upgrade_time);
	auto timelb = Label::createWithSystemFont(str->getCString(),"",24);
	timelb->setColor(Color3B::BLACK);
	tip->PushTopBeginNode( UITipLayer::TopBegin::TopBeginNode3,timelb);
    
	str = String::createWithFormat("%d",soldiercfg->getNextcfg()->upgrade_cost);
	auto btn = UITouchButton::create("ui/107.0.png");
	btn->addChild(Label::createWithSystemFont(str->getCString(),"",10));
	tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode3,btn);
	btn->setPositionX(100);
	btn->setClickTarget(CC_CALLBACK_1(UIResearchDialog::onUpgrade,this));


	//信息上
	auto str1 = String::createWithFormat(CON->getValue("attackadd").asString().c_str(),soldiercfg->getCurcfg()->attack,soldiercfg->getNextcfg()->attack - soldiercfg->getCurcfg()->attack );
	auto label = Label::createWithSystemFont(str1->getCString(),"",24);
	label->setColor(Color3B::BLACK);
	auto pos1 =  Point(tip->getConTentNode()->getContentSize().width/2,tip->getContentInfoNode()->getContentSize().height- label->getContentSize().height/2);
	tip->PushTopBeginNode( UITipLayer::TopBegin::TopBeginNode,label,pos1);

	auto str2 =  String::createWithFormat(CON->getValue("hpadd").asString().c_str(),soldiercfg->getCurcfg()->max_hp,soldiercfg->getNextcfg()->max_hp - soldiercfg->getCurcfg()->max_hp);
	auto labe2 =  Label::createWithSystemFont(str2->getCString(),"",24);
	labe2->setColor(Color3B::BLACK);
	tip->PushTopBeginNode( UITipLayer::TopBegin::TopBeginNode,labe2);

	//信息下
	auto str3   = String::createWithFormat("attack target: %d",soldiercfg->getCurcfg()->attack_target);
	auto label3 = Label::createWithSystemFont(str3->getCString(),"",24); 
	label3->setColor(Color3B::BLACK);
	auto pos2 =  Point(tip->getConTentNode()->getContentSize().width/2,tip->getContentInfoNode()->getContentSize().height/3);
	tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode2,label3,pos2);

	auto str4 = String::createWithFormat("attack type: %d",soldiercfg->getCurcfg()->attack_type);
	auto label4 = Label::createWithSystemFont(str4->getCString(),"",24);
	label4->setColor(Color3B::BLACK);
	tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode2,label4);

	auto str5 = String::createWithFormat("population : %d",soldiercfg->getCurcfg()->population);
	auto label5 = Label::createWithSystemFont(str5->getCString(),"",24);
	label5->setColor(Color3B::BLACK);
	tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode2,label5);

	auto str6 = String::createWithFormat(CON->getValue("traintime").asString().c_str(),soldiercfg->getCurcfg()->train_time);
	auto label6 = Label::createWithSystemFont(str6->getCString(),"",24);
	label6->setColor(Color3B::BLACK);
	tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode2,label6);

	auto str7 = String::createWithFormat("move speed : %d",soldiercfg->getCurcfg()->move_speed);
	auto label7 = Label::createWithSystemFont(str7->getCString(),"",24);
	label7->setColor(Color3B::BLACK);
	tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode2,label7);

	//auto tip = SoldierTip::createSoldierTip(soldiercfg);
	//_tiplayer = tip->getTipLayer();

	
	addChild(tip,100);

}

void UIResearchDialog::updateTimer( float dt )
{
	 auto cfg = SoldierUpgradeData::getSoldierUpgradeMapFromId(_db->id);
	 if (cfg == nullptr || cfg->getSoldierLevel() == nullptr || cfg->getLeftTime() == 0)
	 {   
		 getInfoPic()->removeFromParent();
		 setInfoPic(nullptr);

		 getTimerNode()->removeFromParent();
		 setTimerNode(nullptr);

		 this->unschedule(schedule_selector(UIResearchDialog::updateTimer));
	 }
	 else
	 {
		 auto label = dynamic_cast<Label*>(getTimerNode()->getChildByTag(111));
		 auto str4 = String::createWithFormat(CON->getValue("traintime").asString().c_str(),cfg->getLeftTime());
		 label->setString(str4->getCString());
	 }
}

void UIResearchDialog::onUpgrade( Ref * ref )
{

	//TODO 判定条件
    auto soldiercfg = _selectItem->getData();
	auto curcfg = SoldierUpgradeData::getSoldierUpgradeMapFromId(_db->id);
	if (curcfg && curcfg->getLeftTime() != 0)
	{    
		
	}
	else
	{
		if (soldiercfg->getCurcfg() && soldiercfg->getNextcfg())
		{
			SoldierUpgradeData::UpgradeSoldier(_db->id,soldiercfg->getNextcfg()->id / 100,soldiercfg->getNextcfg()->upgrade_time);
			initUpgradeSoldier(false);
		}
	}
	_tiplayer->removeFromParent();
	_tiplayer = nullptr;
}
