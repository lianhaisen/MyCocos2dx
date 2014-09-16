#include "UIBuyBuildingLayer.h"
#include "UITouchButton.h"
#include "CustomTableViewCell.h"
#include "BuildingConfig.h"
#include "BuildingCommon.h"
#include "BuildingFunc.h"


UIBuyBuildingLayer::UIBuyBuildingLayer()
{
	_bg = nullptr;
	_selectSp = nullptr;
	_tableview = nullptr;
	_curtag = 1;
	_btnCallback = nullptr;
}

UIBuyBuildingLayer::~UIBuyBuildingLayer()
{
    CC_SAFE_RELEASE(_bg);

}
bool UIBuyBuildingLayer::init()
{
	bool bRef = false;
	do{
		CC_BREAK_IF(!LayerColor::initWithColor(Color4B(25, 25, 25, 125)));
		
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(UIBuyBuildingLayer::onTouchesBegan,this);
		listener->onTouchMoved = CC_CALLBACK_2(UIBuyBuildingLayer::onTouchesMoved,this);
		listener->onTouchEnded = CC_CALLBACK_2(UIBuyBuildingLayer::onTouchesEnded,this);
		listener->onTouchCancelled = CC_CALLBACK_2(UIBuyBuildingLayer::onTouchCancelled,this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		initBg();

		bRef = true;
	}while(0);
	return bRef;
}

void UIBuyBuildingLayer::initBg()
{
	setBg(Sprite::create("ui/bank.png"));
	getBg()->setAnchorPoint(Point(0,0));
	addChild(getBg());
    
	//四个标签页
	auto sprite1 = Sprite::create("ui/select.png");
	_curSp = sprite1;
	_taps.pushBack(sprite1);
	_taps.pushBack(Sprite::create("ui/normal.png"));
    _taps.pushBack(Sprite::create("ui/normal.png"));
	_taps.pushBack(Sprite::create("ui/normal.png"));

    auto width    = sprite1->getContentSize().width;
	auto height   = sprite1->getContentSize().height;
	auto beginpos = (Director::getInstance()->getWinSize().width - width * (_taps.size() - 1)) / 2;

	std::vector<std::string> str_vec;
	auto con = Configuration::getInstance();
	str_vec.push_back(con->getValue("text_fundation").asString());
	str_vec.push_back(con->getValue("text_def").asString());
	str_vec.push_back(con->getValue("text_resource").asString());
	str_vec.push_back(con->getValue("text_decorate").asString());

	auto index = 0;
	for (auto sp : _taps)
	{
	   sp->setAnchorPoint(Point(0.5f,0));
	   sp->setPosition(Point(beginpos,0));
	   sp->setTag(index + 1);
	   addChild(sp);
	   
	   auto lable = Label::createWithSystemFont(str_vec.at(index++),"",20);
	   lable->setColor(Color3B(200,200,200));
	   lable->setPosition(Point(width/2,height/2));
	   sp->addChild(lable);

	   beginpos += width;
	}

	_tableview = TableView::create(this, Size(Director::getInstance()->getWinSize().width, 317 + 200));
	_tableview->setDirection(ScrollView::Direction::HORIZONTAL);
	_tableview->setPosition(Point(0,65));
	_tableview->setDelegate(this);
	this->addChild(_tableview);
	_tableview->reloadData();

	

}

bool UIBuyBuildingLayer::onTouchesBegan( Touch * touch, cocos2d::Event *event )
{  
	if (isTouchEdge(touch))
	{
		this->removeFromParentAndCleanup(true);
		return true;
	}

	_selectSp = getItemForTouch(touch);
	if (_selectSp != nullptr && _selectSp != _curSp)
	{
		_selectSp->runAction(ScaleTo::create(0.05f, 0.9f));
	}
	return true;
}

void UIBuyBuildingLayer::onTouchesMoved( Touch * touch, cocos2d::Event *event )
{
	if (_selectSp != nullptr && _selectSp != _curSp)
	{
		if (_selectSp != getItemForTouch(touch))
		{   
			_selectSp->runAction(ScaleTo::create(0.05f, 1.0f));
			_selectSp = nullptr;
		}
	}
}

void UIBuyBuildingLayer::onTouchesEnded( Touch * touch, cocos2d::Event *event )
{
	if (_selectSp != nullptr && _selectSp != _curSp)
	{
		if (_selectSp == getItemForTouch(touch))
		{  
			_selectSp->runAction(ScaleTo::create(0.05f, 1.0f));
			auto selimg =  Director::getInstance()->getTextureCache()->addImage("ui/select.png");
			auto norimg =  Director::getInstance()->getTextureCache()->addImage("ui/normal.png");
			_curSp->setTexture(norimg);
			_curSp  = _selectSp;
			_curSp->setTexture(selimg);
			_curtag = _curSp->getTag();
			_tableview->reloadData();
			_selectSp = nullptr;
		}
		else
		{
			_selectSp->runAction(ScaleTo::create(0.05f, 1.0f));
			_selectSp = nullptr;
		}
	}
}

void UIBuyBuildingLayer::onTouchCancelled( Touch * touch, cocos2d::Event *event )
{
	onTouchEnded(touch,event);
}

Sprite * UIBuyBuildingLayer::getItemForTouch( Touch * touch )
{
	Sprite * ritem = nullptr;
	if (!_taps.empty() && touch)
	{   
		for (auto item : _taps)
		{   
			if (item && item->isVisible())
			{                 
				Point local = convertToNodeSpace(touch->getLocation());
				Rect r = item->getBoundingBox();
				if (r.containsPoint(local))
				{
					ritem = item;
					break;
				}
			}
		}
	}
	return ritem;
}

void UIBuyBuildingLayer::tableCellTouched( TableView* table, TableViewCell* cell )
{
	 //CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size UIBuyBuildingLayer::tableCellSizeForIndex( TableView *table, ssize_t idx )
{
	return Size(264, 317 + 100);
}

TableViewCell* UIBuyBuildingLayer::tableCellAtIndex( TableView *table, ssize_t idx )
{
	auto string = String::createWithFormat("%ld", idx);
	auto cell = dynamic_cast<CustomTableViewCell*>(table->dequeueCell());
	if (!cell)
	{
		cell = CustomTableViewCell::create();
		cell->setCilckTarget(CC_CALLBACK_1(UIBuyBuildingLayer::onCardClick,this));
	}   
	auto buycfg = BuyListConfig::getBuyListFromType( (OpenBuilding::BuyBuildingType)_curtag).at(idx);
	auto con = Configuration::getInstance();

	auto texture = Director::getInstance()->getTextureCache()->addImage(BuildingCommon::getPicFromId(buycfg->buildcfg->id));
	cell->getInfoPic()->setTexture(texture,BuildingCommon::getPicFromId(buycfg->buildcfg->id).c_str());

    cell->getTitle()->setString(buycfg->buildcfg->name);
	auto str = String::createWithFormat("%d",buycfg->buildcfg->cost_value);
	cell->getLableCost()->setString(str->getCString());
	str = String::createWithFormat("%d s",buycfg->buildcfg->cost_time);
	cell->getTimeLable()->setString(str->getCString());
	str = String::createWithFormat("%d/%d",buycfg->current,buycfg->opencfg->getLimitCountFormLevel(BuildingFunc::getInstance()->getMainLevel()));
	cell->getHasBuildLable()->setString(str->getCString());

	//Check is Disable
	if (buycfg->current >= buycfg->opencfg->getLimitCountFormLevel(BuildingFunc::getInstance()->getMainLevel()))
	{  
		auto level = 1;
		for (auto i = level;i <= 10; i++)
		{
			if ( buycfg->opencfg->getLimitCountFormLevel(i) > buycfg->current )
			{   
				level = i;
				break;
			}
		}
       if (level != 1)
       {
		   auto str = String::createWithFormat(con->getValue("levelcanmake").asString().c_str(),level);
		   cell->setDisable(true,str->getCString());
       }
	   else
	   {
		   cell->setDisable(true);
	   }
	}
	else
	{
		cell->setDisable(false);
	}
     
	return cell;
}

ssize_t UIBuyBuildingLayer::numberOfCellsInTableView( TableView *table )
{
	 return BuyListConfig::getBuyListFromType( (OpenBuilding::BuyBuildingType)_curtag).size();
}

void UIBuyBuildingLayer::onCardClick( Ref* sender )
{
    auto card = dynamic_cast<CustomTableViewCell*>(sender);
	//TODO
	//if (_btnCallback && !card->getIsDisable())
	if (_btnCallback)
	{   
		auto buycfg = BuyListConfig::getBuyListFromType( (OpenBuilding::BuyBuildingType)_curtag).at(card->getIdx());
		_btnCallback(this,buycfg);
	}
	this->removeFromParentAndCleanup(true);

}

bool UIBuyBuildingLayer::isTouchEdge(Touch* touch)
{
	Point local = convertToNodeSpace(touch->getLocation());
	auto rect = Rect(0,317 + 100,Director::getInstance()->getWinSize().width,Director::getInstance()->getWinSize().height - 417);
	if (rect.containsPoint(local))
	{   
		return true;
	}
	return false;
}




