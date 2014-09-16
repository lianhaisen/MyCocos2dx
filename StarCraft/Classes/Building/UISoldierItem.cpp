#include "UISoldierItem.h"
#include "BuildingFunc.h"

#define BoxSize Size(86,109)
UISoldierItem::UISoldierItem()
{
	_bg = nullptr;
	_pic = nullptr;
	_touchBtn = nullptr;
	_btnimg = nullptr;
	_levellabel = nullptr;
	_tipLabel = nullptr;
	_status = ItemStatus::NOMAL;
	_soldierlv = nullptr;
	_callback = nullptr;
	_costLabel = nullptr;
}

UISoldierItem::~UISoldierItem()
{
   CC_SAFE_RELEASE(_bg);
   CC_SAFE_RELEASE(_touchBtn);
   CC_SAFE_RELEASE(_pic);
   CC_SAFE_RELEASE(_btnimg);
   CC_SAFE_RELEASE(_levellabel);
   CC_SAFE_RELEASE(_tipLabel);
  
}

void UISoldierItem::initBackGround()
{    
	 
	 setContentSize(BoxSize);
	 if (_soldierlv && _soldierlv->getCurcfg() && _soldierlv->getNextcfg())
	 {
		 if (_soldierlv->getNextcfg()->lab_level > BuildingFunc::getInstance()->getScienceLevel())
		 {
			 _status = ItemStatus::FULL;
		 }
	 }
	 else
	 {
		 _status = ItemStatus::UNENABLE;
	 }

	 

	 if (_status == ItemStatus::UNENABLE)
	 {
		 setBg(Sprite::create("ui/618.0.png"));
		 getBg()->setPosition(BoxSize.width/2,BoxSize.height/2);
		 addChild(getBg());

		 auto picstr = String::createWithFormat("ui/%d.png",_soldierlv->getCurcfg()->id / 100);
		 setPIC(UIGraySprite::create(picstr->getCString()));
		 getPIC()->setGray(true);
		 getPIC()->setPosition(BoxSize.width/2,BoxSize.height/2);
		 addChild(getPIC());
	 }
	 else
	 {   
		 setButton(UITouchButton::create("ui/628.png"));
		 getButton()->setPosition(BoxSize.width/2,BoxSize.height/2);
		 getButton()->setIsUsedForScroller(true);
		 addChild(getButton());


		 auto picstr = String::createWithFormat("ui/%d.png",_soldierlv->getCurcfg()->id / 100);
		 setButtonImg(UIGraySprite::create(picstr->getCString()));
		 _touchBtn->addChild(getButtonImg());

		 auto str = String::createWithFormat("Lv.%d",_soldierlv->getCurcfg()->id % 100);
		 setLevelLabel(Label::createWithSystemFont(str->getCString(),"",20));
         getLevelLabel()->setPositionY(40);
		 _touchBtn->addChild(getLevelLabel());


		 if (_soldierlv->getNextcfg())
		 {
			 auto tipstr = String::createWithFormat("lv.%d can build",_soldierlv->getNextcfg()->lab_level);
			 setTipLabel(Label::createWithSystemFont(tipstr->getCString(),"",10));
			 _touchBtn->addChild(getTipLabel());
			 getTipLabel()->setVisible(false);

			 auto coststr = String::createWithFormat("%d",_soldierlv->getNextcfg()->upgrade_cost);
			 setCostLabel(Label::createWithSystemFont(coststr->getCString(),"",20));
			 getCostLabel()->setPositionY(-40);
			 _touchBtn->addChild(getCostLabel());


		 }
		
		 if (_status == ItemStatus::FULL)
		 {
			 getButton()->getSprite()->setGray(true);
			 getButtonImg()->setGray(true);
			 if (getCostLabel())
			 {
				 getCostLabel()->setVisible(false);
			 }
			 if (getTipLabel())
			 {
				 getTipLabel()->setVisible(true);
			 }
		 }
	 }
	 
      if (getButton())
      {
		  getButton()->setClickTarget(CC_CALLBACK_1(UISoldierItem::onBtnClick,this));
      }
	
     
}

void UISoldierItem::setGray( bool v )
{
	if (v && _status == ItemStatus::NOMAL)
	{   
		getButton()->getSprite()->setGray(true);
		getButtonImg()->setGray(true);
		_status = ItemStatus::FULL;
	}
	else if (!v && _status == ItemStatus::FULL)
	{  
		getButton()->getSprite()->setGray(false);
		getButtonImg()->setGray(false);
		_status = ItemStatus::NOMAL;
	}
	
}

UISoldierItem * UISoldierItem::createFromData( UserSoldierLevel * data )
{
	auto item = UISoldierItem::create();
    item->_soldierlv = data;
	item->initBackGround();
	return item;
}

void UISoldierItem::onBtnClick( Ref * ref )
{
	if (_callback)
	{
		_callback(this);
	}
}

void UISoldierItem::setCallback( const ccMenuCallback& callback )
{
	_callback = callback;
}


UISoldierUpgradeLayer::UISoldierUpgradeLayer()
{

}

UISoldierUpgradeLayer::~UISoldierUpgradeLayer()
{

}

void UISoldierUpgradeLayer::setCallback( const ccMenuCallback& callback )
{
	auto boxsize = Size(90,120);
	auto map = UserSoldierLevel::getUserSoldierLevels();
	int i = 0;
	for (auto pair : map)
	{  

		// TODO 判定条件 确定显示
		auto soldierlv = pair.second;
		auto item = UISoldierItem::createFromData(soldierlv);
		item->setPosition(boxsize.width * (i / 2),boxsize.height * (i % 2));
		item->setCallback(callback);
		addChild(item);
		i++;
	}
	this->setContentSize(Size(90 * (i  / 2  +  i % 2 ) ,240));
}

