#include "UIConveyorLayer.h"
#include "BuildingFunc.h"
#include "SoldierConfig.h"
#include "UISoldierHeadLayer.h"
#include "UITipLayer.h"
UIConveyorLayer::UIConveyorLayer()
{
	m_iCurState=0;
	m_ConveyorInfolist = nullptr;
	m_SoldierInfolist = nullptr;


}
UIConveyorLayer::~UIConveyorLayer()
{

}

 bool UIConveyorLayer::init()
 {
	 if (UITipLayer::init())
	{   
		//this->setVisibleBackBtn(false);
		initWithForm();
		return true;
	}
	 return false;
 }

 void UIConveyorLayer::initWithForm()
 {
	 createConveyorList();
	 showSoldierInfoLayer();
	 if(m_iCurState >= 1 )
	 {
		 showConveyorInfoLayer();
	 }

 }

 void UIConveyorLayer::createConveyorList()
 {
	 auto node = this->getContentInfoNode();
	 auto Nodesize = Size(120,node->getContentSize().height);
	 ListView* listView = ListView::create();
	 // set list view ex direction
	 listView->setDirection(SCROLLVIEW_DIR_VERTICAL);
	 listView->setTouchEnabled(true);
	 listView->setBounceEnabled(true);
	 //listView->setBackGroundImage("ui/1269.0.png");
	 //listView->setBackGroundImageScale9Enabled(true);
	 listView->setSize(Nodesize*0.98f);
	 listView->setPosition(Point(0,5));
	 node->addChild(listView);

	 auto map = BuildingFunc::getInstance()->getAllSoldier();
	
	 int MAXNum= 0;
	 auto it = map.begin();
	 for (it;it != map.end();it++)
	 {
		 auto itme = it->second;
		 if(itme->soldierid /100 == CONVEYORID)
		 {
			 m_iCurState = 1;
			 MAXNum = itme->number;
			 break;
		 }

	 }
	 for (int i = 0; i < MAXNum; i++)
	 {
		 auto btn = UITouchButton::create("btn/btn_hero_head01.png");
		 btn->setIsUsedForScroller(true);

		 Layout* custom_item = Layout::create();
		 custom_item->setSize(btn->getContentSize()*1.1f);
		 btn->setPosition(Point(custom_item->getSize().width / 2.0f, custom_item->getSize().height / 2.0f));
		 custom_item->addChild(btn);
		 listView->pushBackCustomItem(custom_item);

		 ConveyorConfig::CreateItem(i+1); //添加数据

	 }
 }
 //运输机内士兵
 void UIConveyorLayer::showConveyorInfoLayer()
 {
	 auto node = this->getContentInfoNode();
	 auto Nodesize = Size(node->getContentSize().width/2.5,node->getContentSize().height);

	 if(m_ConveyorInfolist)
	 {
		m_ConveyorInfolist->removeAllItems();
	 }
	 else
	 {
		 ListView* listView = ListView::create();
		 // set list view ex direction
		 listView->setDirection(SCROLLVIEW_DIR_VERTICAL);
		 listView->setTouchEnabled(true);
		 listView->setBounceEnabled(true);
		 listView->setBackGroundImage("dian9/huangban.9@2x.png"); //背景图
		 listView->setBackGroundImageScale9Enabled(true);		//是否用的是点9图
		 listView->setSize(Nodesize*0.98f);
		 listView->setPosition(Point(120,5));
		 node->addChild(listView);
		 m_ConveyorInfolist = listView;
	 }
	 int i = 1;
	 auto info = ConveyorConfig::getConveyorConfigFromId(m_iCurState);
	 if(info)
	 {
		 auto  iter = info->soldieritems.begin();
		 for (iter;iter != info->soldieritems.end(); iter++)
		 {
			 auto iteminfo = (*iter);
			 while (iteminfo->number == 0)
			 {
				 iter++;
				 if(iter == info->soldieritems.end())return;
				 iteminfo = (*iter);
			 }

			 auto soldierInfo =  SoldierConfig::getSoldierConfig(iteminfo->soldierid);
			 if(!soldierInfo){
				 return ;
			 }
			 auto layer1 = UISoldierHeadLayer::create(soldierInfo,iteminfo->number,true);
			 layer1->getCurDownLayer()->setVisible(false);
			 layer1->setIsTopVisible(false);
			 layer1->setRecruitCallBack(CC_CALLBACK_1( UIConveyorLayer::ReductionSoldier,this));
			 layer1->setTag(i);

			 iter++;
			 i++;
			 if(iter != info->soldieritems.end())
			 {
				 auto iteminfo = (*iter);
				 while (iteminfo->number == 0)
				 {
					 iter++;
					 if(iter == info->soldieritems.end()){				 
						 auto tmpnode = getOneItem(layer1,nullptr);
						 Layout* custom_item = Layout::create();
						 custom_item->setSize(tmpnode->getContentSize());
						 tmpnode->setPosition(Point(custom_item->getSize().width / 2.0f-tmpnode->getContentSize().width/2, custom_item->getSize().height / 2.0f-tmpnode->getContentSize().height/2));
						 custom_item->addChild(tmpnode);

						 m_ConveyorInfolist->pushBackCustomItem(custom_item);
						 return;
					 }
					 iteminfo = (*iter);
				 }
				 soldierInfo =  SoldierConfig::getSoldierConfig(iteminfo->soldierid);
				 if(!soldierInfo){
					 return ;
				 }
				 auto layer2 = UISoldierHeadLayer::create(soldierInfo,iteminfo->number,true);
				 layer2->getCurDownLayer()->setVisible(false);
				 layer2->setIsTopVisible(false);
				 layer2->setRecruitCallBack(CC_CALLBACK_1( UIConveyorLayer::ReductionSoldier,this));
				 layer2->setTag(i);
				 auto tmpnode = getOneItem(layer1,layer2);
				 Layout* custom_item = Layout::create();
				 custom_item->setSize(tmpnode->getContentSize());
				 tmpnode->setPosition(Point(custom_item->getSize().width / 2.0f-tmpnode->getContentSize().width/2, custom_item->getSize().height / 2.0f-tmpnode->getContentSize().height/2));
				 custom_item->addChild(tmpnode);

				 m_ConveyorInfolist->pushBackCustomItem(custom_item);

			 }
			 else 
			 {
				 auto tmpnode = getOneItem(layer1,nullptr);
				 Layout* custom_item = Layout::create();
				 custom_item->setSize(tmpnode->getContentSize());
				 tmpnode->setPosition(Point(custom_item->getSize().width / 2.0f-tmpnode->getContentSize().width/2, custom_item->getSize().height / 2.0f-tmpnode->getContentSize().height/2));
				 custom_item->addChild(tmpnode);

				 m_ConveyorInfolist->pushBackCustomItem(custom_item);
				 return ;
			 }
		 }
	 }

 }



 void UIConveyorLayer::showSoldierInfoLayer()
 {
	 auto node = this->getContentInfoNode();
	 auto Nodesize = Size(node->getContentSize().width/2.5,node->getContentSize().height);
	 if(m_SoldierInfolist)
	 {
		m_SoldierInfolist->removeAllItems();
	 }
	 else 
	 {
		 ListView* listView = ListView::create();
		 // set list view ex direction
		 listView->setDirection(SCROLLVIEW_DIR_VERTICAL);
		 listView->setTouchEnabled(true);
		 listView->setBounceEnabled(true);
		 listView->setBackGroundImage("dian9/huangkuang.9@2x.png"); //背景图
		 listView->setBackGroundImageScale9Enabled(true);		//是否用的是点9图
		 listView->setSize(Nodesize*0.98f);
		 listView->setPosition(Point(125+Nodesize.width,5));
		 node->addChild(listView);
		 m_SoldierInfolist = listView;
	 }
	 auto map = BuildingFunc::getInstance()->getAllSoldier();
	 int i = 1;
	 auto iter = map.begin();
	 for(iter ;iter != map.end() ;iter++)
	 {
		 auto info = iter->second;
		 while (info->number-info->soldierstate == 0)
		 {
			iter++;
			if(iter == map.end())return;
			info = iter->second;
		 }
		 auto soldierInfo =  SoldierConfig::getSoldierConfig(info->soldierid);
		 if(!soldierInfo){
			 return ;
		 }
		 auto layer1 = UISoldierHeadLayer::create(soldierInfo,info->number-info->soldierstate,true);
		 layer1->getCurDownLayer()->setVisible(false);
		 layer1->setIsTopVisible(false);
		 layer1->setRecruitCallBack(CC_CALLBACK_1( UIConveyorLayer::addSoldier,this));
		 iter++;
		 i++;

		 if(iter != map.end() )
		 {
			 info = iter->second;
			 while (info->number-info->soldierstate == 0)
			 {
				 iter++;
				 if(iter == map.end()){
					 auto tmpnode = getOneItem(layer1,nullptr);
					 Layout* custom_item = Layout::create();
					 custom_item->setSize(tmpnode->getContentSize());
					 tmpnode->setPosition(Point(custom_item->getSize().width / 2.0f-tmpnode->getContentSize().width/2, custom_item->getSize().height / 2.0f-tmpnode->getContentSize().height/2));
					 custom_item->addChild(tmpnode);

					 m_ConveyorInfolist->pushBackCustomItem(custom_item);
					 return;
				 }
				 info = iter->second;
			 }
			 soldierInfo =  SoldierConfig::getSoldierConfig(info->soldierid);
			 if(!soldierInfo){
				 return ;
			 }
			 auto layer2 = UISoldierHeadLayer::create(soldierInfo,info->number-info->soldierstate,true);
			 layer2->getCurDownLayer()->setVisible(false);
			 layer2->setIsTopVisible(false);
			 layer2->setRecruitCallBack(CC_CALLBACK_1( UIConveyorLayer::addSoldier,this));
			 auto tmpnode = getOneItem(layer1,layer2);
			 Layout* custom_item = Layout::create();
			 custom_item->setSize(tmpnode->getContentSize());
			 tmpnode->setPosition(Point(custom_item->getSize().width / 2.0f-tmpnode->getContentSize().width/2, custom_item->getSize().height / 2.0f-tmpnode->getContentSize().height/2));
			 custom_item->addChild(tmpnode);

			 m_SoldierInfolist->pushBackCustomItem(custom_item);

		 }
		 else 
		 {
			 auto tmpnode = getOneItem(layer1,nullptr);
			 Layout* custom_item = Layout::create();
			 custom_item->setSize(tmpnode->getContentSize());
			 tmpnode->setPosition(Point(custom_item->getSize().width / 2.0f-tmpnode->getContentSize().width/2, custom_item->getSize().height / 2.0f-tmpnode->getContentSize().height/2));
			 custom_item->addChild(tmpnode);

			 m_SoldierInfolist->pushBackCustomItem(custom_item);
			 return ;
		 }

	 }
 }

 Node* UIConveyorLayer::getOneItem( Node* Item1,Node* Item2 )
 {
	 auto node = Node::create();
	 node->setContentSize(Size(Item1->getContentSize().width*2+10,Item1->getContentSize().height+10));
	 node->addChild(Item1);
	 Item1->setAnchorPoint(Point(0,0.5));
	 Item1->setPosition(Item1->getContentSize().width/2-10,node->getContentSize().height/2);

	/* auto lauer = LayerColor::create(Color4B(255,255,100,125));
	 node->addChild(lauer);*/
	 if(Item2){
		 node->addChild(Item2);
		 Item2->setAnchorPoint(Point(0,0.5));
		 Item2->setPosition(Item1->getPositionX()+Item1->getContentSize().width+20,node->getContentSize().height/2);
	 }
	 return node;
 }

 void UIConveyorLayer::addSoldier( Ref *sender )
 {
	 auto layer =  static_cast<UISoldierHeadLayer*>(sender);
	 UpDataConveyorInfo(m_iCurState,layer->getData()->id);
 }

 void UIConveyorLayer::UpDataConveyorInfo(int id, int TypeId )
 {
	 //添加运输机内士兵  
	 auto info = ConveyorConfig::getConveyorConfigFromId(id);
	 int Barrackid = 0;
	 if(info)
	 {
		 auto tmpmap = BarrackData::getBarrackDataMap();
		 bool boolen = true;
		 for (auto item:tmpmap)
		 {
			 for(auto item2:item.second->soldieritems)
			 {
				 if(item2->soldierid == TypeId && boolen)
				 {

					 if(item2->soldierstate < item2->number)
					 {
						 Barrackid = item2->id;
						 boolen = false;
					 }
					 else break;
				 }

			 }
		 }
		 if(Barrackid == 0 )return;
		 info->addSoldier(Barrackid,TypeId);
		 //减少可用士兵
		 auto map = BarrackData::getBarrackDataMap();
		 for (auto item:map)
		 {
			 auto Soldier = item.second;
			 if(Soldier->id == Barrackid){
				 Soldier->updataSoldierUseState(TypeId,Barrackid,1);
				 auto tmpmap = BuildingFunc::getInstance()->getAllSoldier();
				 auto iter = tmpmap.find(TypeId);
				 if(iter != tmpmap.end())
					iter->second->soldierstate++;
				 break;
			 }
		 }

	 }

	 //刷新界面
	 showConveyorInfoLayer();
	 showSoldierInfoLayer();
 }



 void UIConveyorLayer::ReductionSoldier( Ref *sender )
 {
	 auto layer =  static_cast<UISoldierHeadLayer*>(sender);
	 UpDataSoldierInfo(m_iCurState,layer->getData()->id);
 }


 void UIConveyorLayer::UpDataSoldierInfo( int id,int TypeId )
 {
	 //减少运输机内士兵  
	 auto info = ConveyorConfig::getConveyorConfigFromId(id);
	 int Barrackid = 0;
	 if(info)
	 {
		 auto tmpmap = BarrackData::getBarrackDataMap();
		 bool boolen = true;
		 for (auto item:tmpmap)
		 {
			 for(auto item2:item.second->soldieritems)
			 {
				 if(item2->soldierid == TypeId && boolen)
				 {

					 if(item2->soldierstate <= item2->number && item2->soldierstate != 0 )
					 {
						 Barrackid = item2->id;
						 boolen = false;
					 }
					 else break;
				 }

			 }
		 }
		 if(Barrackid == 0) return;
		 info->DeleteSoldier(Barrackid,TypeId);
		 //添加可用士兵 
		 auto map = BarrackData::getBarrackDataMap();
		 for (auto item:map)
		 {
			 auto Soldier = item.second;
			 if(Soldier->id == Barrackid){
				 Soldier->updataSoldierUseState(TypeId,Barrackid,-1);
				 auto tmpmap = BuildingFunc::getInstance()->getAllSoldier();
				 auto iter = tmpmap.find(TypeId);
				 if(iter != tmpmap.end())
					 iter->second->soldierstate--;
				 break;
			 }
		 }

	 }

	 //刷新界面
	 showConveyorInfoLayer();
	 showSoldierInfoLayer();
 }


 