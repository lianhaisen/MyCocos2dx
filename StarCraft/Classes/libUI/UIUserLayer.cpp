#include "UIUserLayer.h"
#include "AppMacros.h"
#include "GameString.h"
#include "UIGraySprite.h"
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
#include "UIUserMinUi.h"

UIUserLayer::UIUserLayer(void)
{
	_popLayer = nullptr;
	_selectDB = nullptr;
}

UIUserLayer::~UIUserLayer(void)
{
}

// on "init" you need to initialize your instance
bool UIUserLayer::init()
{
    if ( !CCScene::init() )
    {
        return false;
    }
	TestPlaceBuilding();
	//TestLogic2();
	//createUIScene();
    return true;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
}
 
void UIUserLayer::doLongPress( PointArray* array )
{   

}

void UIUserLayer::doClick( PointArray* array )
{

}

bool UIUserLayer::checkIsCanClickOrLongPress( PointArray* array )
{
	return false;
}

void UIUserLayer::buildingSelect( Ref* sender )
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

	    _popLayer->addButtons(UIButtonFactory::getButtonsFromDB(_selectDB,CC_CALLBACK_1(UIUserLayer::buttonClick,this)));


		_popLayer->setStatus(UIBuildingPopLayer::PopStatus::SHOW);
	}
	else
	{    
		_popLayer->setStatus(UIBuildingPopLayer::PopStatus::UNSHOW);
	}
	
}

void UIUserLayer::buttonClick( Ref * ref )
{   
	auto btn = dynamic_cast<UITouchButton *>(ref);
	auto layer = UIPopLayerFactory::createLayerFromTypeAndDB((UIButtonFactory::UIButtonType)btn->getTag(),_selectDB);
	if (layer)
	{
		_popLayer->setStatus(UIBuildingPopLayer::PopStatus::UNSHOW);
		layer->setCloseTarget(CC_CALLBACK_1(UIUserLayer::dlgClose,this));
		addChild(layer);
	}
}

void UIUserLayer::dlgClose( Ref * ref )
{   
	_popLayer->addButtons(UIButtonFactory::getButtonsFromDB(_selectDB,CC_CALLBACK_1(UIUserLayer::buttonClick,this)));
	_popLayer->setStatus(UIBuildingPopLayer::PopStatus::SHOW);
    auto node = dynamic_cast<Node *>(ref);
    node->removeFromParentAndCleanup(true);
}

void UIUserLayer::TestPlaceBuilding()
{    auto visibleSize = Director::getInstance()->getVisibleSize(); //窗口大小
	auto origin = Director::getInstance()->getVisibleOrigin();
	 //建筑层
    auto data = BuildingData::getBuildingData();
    _zoomlayer = BuildingCommon::getPlayer(false,&data,CC_CALLBACK_1(UIUserLayer::buildingSelect,this));
	 addChild(_zoomlayer);

     //按钮弹出层
	 _popLayer = UIBuildingPopLayer::create();
	 addChild(_popLayer);

	 //建造按钮
	 auto btn = UITouchButton::create("btn/btn_building.png");
	 btn->setPosition(Point(btn->getContentSize().width/2 ,btn->getContentSize().height/2));
	 btn->setClickTarget(CC_CALLBACK_1(UIUserLayer::onBuy,this));
	 addChild(btn);

	 //主界面UI
	auto layer = UIUserMinUi::getInstance();
	addChild(layer);
}

void UIUserLayer::onBuy( Ref * ref )
{
	auto layer = UIBuyBuildingLayer::create();
	layer->setTarget(CC_CALLBACK_2(UIUserLayer::onBuyReturn,this));
	addChild(layer);
}

void UIUserLayer::TestLogic2()
{
	//std::vector<BBuilding*> database;
	//database.push_back(new BBuilding(1,74000301,16,22,0,100));
	//database.push_back(new BBuilding(2,74000301,17,19,0,100));
 //   database.push_back(new BBuilding(3,74000301,14,19,0,100));

	//std::map<int,BattleLogic*> logiclist;
	//auto layer = BuildingCommon::getPlayer2(database,logiclist);
	//addChild(layer);
}

void UIUserLayer::onBuyReturn( Ref * ref,BuyListConfig * cfg )
{  
	auto building = UIBuildingFactory::CreateBuildingFromBuyDB(cfg);
	_zoomlayer->addNewBuilding(building);
	building->setClickTarget(CC_CALLBACK_1(UIUserLayer::buildingSelect,this));

}

