#include "LHSTestScene.h"
#include "AppMacros.h"
#include "GameString.h"
#include "UIGraySprite.h"
#include "UITouchButton.h"
#include "UIPanZoomLayer.h"
#include "BuildingConfig.h"
#include "UIPlaceBuildingLayer.h"
#include "UIBuildingPopLayer.h"
#include "BuildingData.h"
#include "UIInfoDialog.h"
#include "UIBuildingFactory.h"
#include "UIUpgradeDialog.h"
#include "BuildingCommon.h"
#include "UIButtonFactory.h"
#include "UIPopLayerFactory.h"
#include "UIBuyBuildingLayer.h"
#include "UIBuildingFactory.h"
#include "BuildingFunc.h"
#include "UIRecruitLayer.h"
#include "BattleScene.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d::extension;



TestScene::TestScene(void)
{
	_popLayer = nullptr;
	_selectDB = nullptr;
	_miniUI = nullptr;
}

TestScene::~TestScene(void)
{
}

// on "init" you need to initialize your instance
bool TestScene::init()
{
    if ( !CCScene::init() )
    {
        return false;
    }
	TestPlaceBuilding();
	//TestLogic2();
	//TestMainMap();
    return true;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
}
 
void TestScene::doLongPress( PointArray* array )
{   

}

void TestScene::doClick( PointArray* array )
{

}

bool TestScene::checkIsCanClickOrLongPress( PointArray* array )
{
	return false;
}

void TestScene::buildingSelect( Ref* sender )
{
	auto build = dynamic_cast<Building *>(sender);
	_selectDB = build->getBuidingData();
	if (build->isSelected())
	{    
	  
		char sz[100];
		sprintf(sz,Configuration::getInstance()->getValue("des").asString().c_str(),
			       _selectDB->getCurBindCfg()->name.c_str(),
				   _selectDB->getCurBindCfg()->id % 100);
		auto label = Label::createWithSystemFont(sz,"",20);
		_popLayer->setTitleLabel(label);

	    _popLayer->addButtons(UIButtonFactory::getButtonsFromDB(_selectDB,CC_CALLBACK_1(TestScene::buttonClick,this)));


		_popLayer->setStatus(UIBuildingPopLayer::PopStatus::SHOW);
	}
	else
	{    
		_popLayer->setStatus(UIBuildingPopLayer::PopStatus::UNSHOW);
	}
	
}

void TestScene::buttonClick( Ref * ref )
{   
	auto btn = dynamic_cast<UITouchButton *>(ref);
	auto layer = UIPopLayerFactory::createLayerFromTypeAndDB((UIButtonFactory::UIButtonType)btn->getTag(),_selectDB,btn);
	if (layer)
	{
		_popLayer->setStatus(UIBuildingPopLayer::PopStatus::UNSHOW);
		layer->setCloseTarget(CC_CALLBACK_1(TestScene::dlgClose,this));
		addChild(layer);
	}
}

void TestScene::dlgClose( Ref * ref )
{   
	_popLayer->addButtons(UIButtonFactory::getButtonsFromDB(_selectDB,CC_CALLBACK_1(TestScene::buttonClick,this)));
	_popLayer->setStatus(UIBuildingPopLayer::PopStatus::SHOW);
    auto node = dynamic_cast<Node *>(ref);
    node->removeFromParentAndCleanup(true);
}

void TestScene::TestPlaceBuilding()
{    
	 //建筑层
    auto data = BuildingData::getBuildingData();
    _zoomlayer = BuildingCommon::getPlayer(false,&data,CC_CALLBACK_1(TestScene::buildingSelect,this));
	 addChild(_zoomlayer);

     //按钮弹出层
	 _popLayer = UIBuildingPopLayer::create();
	 addChild(_popLayer);

	 //建造按钮
	 auto btn = UITouchButton::create("ui/littelbutton.png");
	 auto label = Label::createWithSystemFont("build","",20);
	 btn->setPosition(Point(btn->getContentSize().width/2,btn->getContentSize().height/2));
	 btn->setClickTarget(CC_CALLBACK_1(TestScene::onBuy,this));
	 btn->addChild(label);
	 addChild(btn);

	 //主界面层
	 _miniUI = UIUserMinUi::create();
	 BuildingFunc::getInstance()->setMinUI(_miniUI);
	 _miniUI->setStaticWarCallBack(CC_CALLBACK_1(TestScene::onWarClick,this));
	 addChild(_miniUI);

	
}

void TestScene::onBuy( Ref * ref )
{
	auto layer = UIBuyBuildingLayer::create();
	layer->setTarget(CC_CALLBACK_2(TestScene::onBuyReturn,this));
	addChild(layer);

	//auto layer = UIRecruitLayer::create();
	//addChild(layer);
}

void TestScene::TestLogic2()
{
	//vector<Building*> ve;
	//auto layer = BuildingCommon::getPlayer3(BuildingData::getBuildingData(),ve);
	//addChild(layer);
}

void TestScene::onBuyReturn( Ref * ref,BuyListConfig * cfg )
{  
	auto building = UIBuildingFactory::CreateBuildingFromBuyDB(cfg);
	_zoomlayer->addNewBuilding(building);
	building->setClickTarget(CC_CALLBACK_1(TestScene::buildingSelect,this));

}

void TestScene::TestMainMap()
{
	

	auto layer = Layer::create();
	layer->setContentSize(Size(4608,2548));

	auto sky01 = Sprite::create("background/sky01.jpg");
	sky01->setAnchorPoint(Point(1,0));
	sky01->setPosition(4608,0);
	layer->addChild(sky01);

	auto sky02 = Sprite::create("background/sky02.jpg");
	sky02->setAnchorPoint(Point(0,1));
	sky02->setPosition(0,2548);
	layer->addChild(sky02);

	auto sky03 = Sprite::create("background/sky03.jpg");
	sky03->setAnchorPoint(Point(0,1));
	sky03->setPosition(2048,2548);
	layer->addChild(sky03);

	auto sky04 = Sprite::create("background/sky04.jpg");
	sky04->setAnchorPoint(Point(1,1));
	sky04->setPosition(4608,2548);
	layer->addChild(sky04);

	auto home01a = Sprite::create("background/home01a.png");
	home01a->setAnchorPoint(Point::ZERO);
	home01a->setPosition(Point::ZERO);
	layer->addChild(home01a);

	auto home01b = Sprite::create("background/home01b.png");
	home01b->setAnchorPoint(Point::ZERO);
	home01b->setPosition(Point(1024,0));
	layer->addChild(home01b);

	auto home02 = Sprite::create("background/home02.png");
	home02->setAnchorPoint(Point::ZERO);
	home02->setPosition(Point(2048,0));
	layer->addChild(home02);

	auto home03 = Sprite::create("background/home03.png");
	home03->setAnchorPoint(Point(0,1));
	home03->setPosition(Point(0,2548));
	layer->addChild(home03);

	auto home04 = Sprite::create("background/home04.png");
	home04->setAnchorPoint(Point(0,0));
	home04->setPosition(Point(2048,2548 - 500));
	layer->addChild(home04);
	

	layer->setScale(0.3f);


	auto scroll = ScrollView::create();
	scroll->setPosition(Point::ZERO);
	scroll->setContentOffset(Point::ZERO);
	scroll->setViewSize(Size(1024,768));
    scroll->setContainer(layer);
	scroll->setDirection(ScrollView::Direction::BOTH);
	addChild(scroll);

}

void TestScene::onWarClick( Ref * ref )
{   
	auto node = dynamic_cast<Node *>(ref);
	auto tag = (UIUserMinUi::MINDOWN_UI_ID) node->getTag();  
	switch (tag)
	{
	case UIUserMinUi::MINDOWN_UI_ID::DownUI_PK:
		break;
	case UIUserMinUi::MINDOWN_UI_ID::DownUI_StarWar:
		{   
			auto scene = BattleScene::create();
			Director::getInstance()->replaceScene(scene);
		}
		break;
	case UIUserMinUi::MINDOWN_UI_ID::DownUI_NPC:
		break;
	case UIUserMinUi::MINDOWN_UI_ID::DownUI_Friendly:
		break;
	default:
		break;
	}

}












