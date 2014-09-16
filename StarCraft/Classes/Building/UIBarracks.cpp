#include "UIBarracks.h"
#include "BuildingFunc.h"
#include "SoldierConfig.h"
#include "UISoldierHeadLayer.h"
#include "UITipLayer.h"

#include "UIConveyorLayer.h"
UIBarracks::UIBarracks()
{


}
UIBarracks::~UIBarracks()
{

}

 bool UIBarracks::init()
 {
	 if (UIMsgLayer::init())
	{   
		this->setVIsibelLeftRightBtn(false);
		this->getContentScrollView()->setTouchEnabled(true);
		this->setCloseTarget(CC_CALLBACK_0(UIBarracks::onClickClose,this));
		initWithForm();
		return true;
	}
	 return false;
 }

 void UIBarracks::initWithForm()
 {
	 auto node = this->getContentInfoNode();

	 auto map = BuildingFunc::getInstance()->getAllSoldier();
	 float Width = 0;
	 float Height = 0;
	 int inum = 0;
	 if(map.size()%8 != 0)
	 {
		 inum = 1;
	 }
	 if(map.size()/8+inum <= 1)
	 {
		 this->getContentScrollView()->setTouchEnabled(false);

	 }
	 this->getContentScrollView()->setContentSize(Size(node->getContentSize().width*(map.size()/8+inum),node->getContentSize().height));
	 int i = 0;
	 int Page = 0;
	 for (auto item:map)
	 {
		auto info = item.second;
		auto soldierInfo =  SoldierConfig::getSoldierConfig(info->soldierid);
		if(soldierInfo){
			int index = i-(Page)*8;
			auto layer2 = UISoldierHeadLayer::create(soldierInfo,info->number,true);
			node->addChild(layer2);
			if(index < 4)
			{
				Width = (index+(Page)*4)*(node->getContentSize().width/4)+layer2->getContentSize().width/1.5;
				Height = node->getContentSize().height/1.5 + layer2->getContentSize().height/2;
			}
			else
			{
				int page = index-4;
				Width = (page+(Page)*4)*(node->getContentSize().width/4)+layer2->getContentSize().width/1.5;
				Height = node->getContentSize().height/3;
			}
			layer2->setPosition(Width,Height);
			layer2->getSoldierinfo()->btn->setEnabled(false);
			layer2->setTipCallBack(CC_CALLBACK_1(UIBarracks::onClickCreateTip,this));

			auto DownLayer = layer2->getCurDownLayer();
			DownLayer->removeAllChildren();
			if(i == 0 ){
				auto btn = UITouchButton::create("btn/btn_jiasu.png");
				btn->setPosition(0,-btn->getContentSize().height/2);
				btn->setClickTarget(CC_CALLBACK_0(UIBarracks::onClicshowConveyor,this));
				DownLayer->addChild(btn);
			}
			i++;
			if(i%8 == 0){Page++;}
		}

		
	 }
 }

 void UIBarracks::onClickCreateTip( Ref *sender )
 {
	 auto item =  static_cast<UISoldierHeadLayer*>(sender);
	 auto tag = item->getSoldierinfo()->nType;
	 auto tip = SoldierTip::createSoldierTip(item->getData());
	 addChild(tip->getTipLayer(),100);
 }

 void UIBarracks::onClickClose()
 {
	 this->removeFromParent();
 }

 void UIBarracks::onClicshowConveyor()
 {
	auto layer =  UIConveyorLayer::create();
	addChild(layer);
 }

 