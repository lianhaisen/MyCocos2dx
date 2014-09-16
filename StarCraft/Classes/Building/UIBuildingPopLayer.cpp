#include "UIBuildingPopLayer.h"
#include "UITouchButton.h"


UIBuildingPopLayer::UIBuildingPopLayer()
{
	_isActionEnd = true;
	_titlelabel = nullptr;

}

UIBuildingPopLayer::~UIBuildingPopLayer()
{
	log("~UIBuildingPopLayer");
	CC_SAFE_RELEASE(_titlelabel);
}

void UIBuildingPopLayer::onEnter()
{
	CCLayer::onEnter();	
}

void UIBuildingPopLayer::onExit()
{   
	CCLayer::onExit();
}

bool UIBuildingPopLayer::init()
{
	bool bRef = false;
	do{
		CC_BREAK_IF(!CCLayer::init());
		scheduleUpdate();
		bRef = true;
	}while(0);
	return bRef;
}


void UIBuildingPopLayer::actionCallBack(Node* node)
{
	CCLayer::setVisible(false);
	log("close callback!");
}

void UIBuildingPopLayer::doShow()
{   
	if (getTitleLabel()!= nullptr)
	{   
		getTitleLabel()->setPosition(Point( Director::getInstance()->getWinSize() .width / 2, 230));
        this->addChild(getTitleLabel());   
	}
	if (_buttons.size() > 0)
	{   
		auto btn =    _buttons.at(0);
		float Width =  btn->getContentSize().width;
		float Hspace = btn->getContentSize().width / 5;
		float totalwidth = Width * _buttons.size() + Hspace * (_buttons.size() - 1);
		float begin = Director::getInstance()->getWinSize() .width / 2 - totalwidth / 2 + Width / 2; 
		float begintime =  0.1  * _buttons.size();
		int i = 0;
		for (auto btn : _buttons)
		{
			btn->setPosition(Point(begin + (Width + Hspace) * i ,-200.0f));
			Sequence * se = Sequence::create(DelayTime::create(begintime - 0.1f * i),
				MoveTo::create(0.1f,Point(btn->getPositionX() ,170)),
				MoveTo::create(0.2f,Point(btn->getPositionX(),150)),
				NULL
				);
			if (i == 0)
			{
				se = Sequence::createWithTwoActions(se,CallFunc::create(CC_CALLBACK_0(UIBuildingPopLayer::showBtnEnd,this)));
			}
			btn->runAction(se);
			i++;
		}

	}
}

void UIBuildingPopLayer::doUnShow()
{   
	if (getTitleLabel()!= nullptr)
	{   
		getTitleLabel()->removeFromParent();
	}

	if (_buttons.size() > 0)
	{   
		int i = 0;
		for (auto btn : _buttons)
		{
			Sequence * se = Sequence::create(DelayTime::create(0.02f * i),
				MoveTo::create(0.2f,Point(btn->getPositionX(),-200)),
				NULL
				);
			if (i == _buttons.size() - 1)
			{   
				se = Sequence::createWithTwoActions(se,
					 CallFunc::create(CC_CALLBACK_0(UIBuildingPopLayer::unshowBtnEnd,this))
					);
			}
			btn->runAction(se);
			i++;
		}
	}
}

void UIBuildingPopLayer::update( float dt )
{
	
	if (_isActionEnd && !_statusvec.empty() && _statusvec[0] ==  PopStatus::UNSHOW  )
	{     
		  _isActionEnd = false;
		   doUnShow();
		  _statusvec.erase(_statusvec.cbegin());
		  return;
		
	}
	else if (_isActionEnd && !_tempbuttons.empty())
	{
		if (!_buttons.empty())
		{
			for (auto item : _buttons)
			{
				item->removeFromParent();
			}
			_buttons.clear();
		}
		for (auto item : _tempbuttons)
		{
			_buttons.pushBack(item);
			item->setPosition(Point(-50,-50));
			this->addChild(item);
		}
		_tempbuttons.clear();
		return;
	}
	else if (_isActionEnd && !_statusvec.empty() && _statusvec[0] ==  PopStatus::SHOW  )
	{
		  _isActionEnd = false;
		  doShow();
		  _statusvec.erase(_statusvec.cbegin());
		  return;
	}
}

void UIBuildingPopLayer::showBtnEnd()
{
	_isActionEnd = true;
}

void UIBuildingPopLayer::unshowBtnEnd()
{
	_isActionEnd = true;
}

void UIBuildingPopLayer::addButtons( Vector<UITouchButton*> buttons )
{ 
 	
	if (!_tempbuttons.empty())
	{   
		_tempbuttons.clear();
	}
	for (auto btn : buttons)
	{
		_tempbuttons.pushBack(btn);
	}
}

Label* UIBuildingPopLayer::getTitleLabel()
{
	return _titlelabel;
}

void UIBuildingPopLayer::setTitleLabel( Label* lb )
{
	if (_titlelabel != lb)
	{
		CC_SAFE_RETAIN(lb);
		if (_titlelabel != nullptr)
		{
			_titlelabel->removeFromParent();
		}
		CC_SAFE_RELEASE(_titlelabel);
		_titlelabel = lb;
	}
}

