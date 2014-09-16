#include "UIRadioButton.h"



UIRadioButton *UIRadioButton::create(MenuItem *item, ...)
{  
	va_list args;  
	va_start(args,item);  

	UIRadioButton *pRet = new UIRadioButton();  

	Vector<MenuItem*> items;
	if (item)
	{
		MenuItem *i = va_arg(args, MenuItem*);
		while(i)  
		{  
			items.pushBack(i);
			i = va_arg(args, MenuItem*);  
		}  
	}
	if (pRet && pRet->initWithArray(items))
	{  
		pRet->_menuItems = items;
		pRet->setSelectedIndex(0);
		pRet->autorelease();  
		va_end(args);  
		return pRet;  
	}

	va_end(args);  
	CC_SAFE_DELETE(pRet);  
	return NULL;  
}  

UIRadioButton* UIRadioButton::createWithArray(const Vector<MenuItem*>& arrayOfItems)
{
	UIRadioButton *pRet = new UIRadioButton();
	if (pRet && pRet->initWithArray(arrayOfItems))
	{
		//pRet->_menuItems = CCArray::createWithArray(pArrayOfItems);
		pRet->_menuItems = arrayOfItems;
		pRet->setSelectedIndex(0);
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

void UIRadioButton::setSelectedItem(MenuItem *item){  
	_selectedItem = NULL;  
	_selectedItem = item;  
	item->selected();
}  

void UIRadioButton::setSelectedIndex(int index)
{
	if (index >= 0 && index < _menuItems.size())
	{
		MenuItem *item = (MenuItem *)_menuItems.at(index);
		_selectedItem = NULL;  
		_selectedItem = item; 
		item->selected();
	}
}

/*
bool UIRadioButton::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{  
	if (m_eState != kCCMenuStateWaiting) return false;  
	CCMenuItem *curSelection = this->itemForTouch(pTouch);  
	if (curSelection) 
	{  
		curSelection->selected();  
		_curHighlighted = curSelection;  

		if (_curHighlighted) 
		{  
			if (m_pSelectedItem != curSelection) 
			{  
				m_pSelectedItem->unselected();  
			}  
			m_eState = kCCMenuStateTrackingTouch;  
			return true;  
		}  
	}  
	return false;  
}  

void UIRadioButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{  
	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");  
	CCMenuItem *curSelection = this->itemForTouch(pTouch);  
	if (curSelection != _curHighlighted && curSelection != NULL) 
	{  
		m_pSelectedItem->selected();  
		_curHighlighted->unselected();  
		_curHighlighted = NULL;  
		m_eState = kCCMenuStateWaiting;  
		return;  
	}  

	m_pSelectedItem = _curHighlighted;  
	_curHighlighted->activate();  
	_curHighlighted = NULL;  

	m_eState = kCCMenuStateWaiting;  
}  

void UIRadioButton::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{  
	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");  
	m_pSelectedItem->selected();  
	_curHighlighted->unselected();  
	_curHighlighted = NULL;  

	m_eState = kCCMenuStateWaiting;  
}  

void UIRadioButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{  
	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");  
	CCMenuItem *curSelection = this->itemForTouch(pTouch);  
	if (curSelection != _curHighlighted && curSelection != NULL) 
	{  
		_curHighlighted->unselected();  
		curSelection->selected();  
		_curHighlighted = curSelection;  
		return;  
	}  
}  
*/

void UIRadioButton::onEnter()
{
	Menu::onEnter();

	// Register Touch Event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(Menu::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Menu::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Menu::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(Menu::onTouchCancelled, this);

	 _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void UIRadioButton::onExit()
{
	//    auto director = Director::getInstance();
	//    director->getTouchDispatcher()->removeDelegate(this);
	Menu::onExit();
} 

bool UIRadioButton::onTouchBegan(Touch* touch, Event* event)
{  
	if (_state != Menu::State::WAITING || ! _visible || !_enabled)
	{
		return false;
	}
	MenuItem *curSelection = this->getItemForTouch(touch);
	if (curSelection) 
	{  
		curSelection->selected();  
		_curHighlighted = curSelection;  

		if (_curHighlighted) 
		{  
			if (_selectedItem != curSelection) 
			{  
				_selectedItem->unselected();  
			}  
			_state = Menu::State::TRACKING_TOUCH;
			return true;  
		}  
	}  
	return false;  
}  

void UIRadioButton::onTouchEnded(Touch* touch, Event* event)
{  
	CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchEnded] -- invalid state");
	this->retain();
	MenuItem *curSelection = this->getItemForTouch(touch);
	if (curSelection != _curHighlighted && curSelection != NULL) 
	{  
		_selectedItem->selected();  
		_curHighlighted->unselected();  
		_curHighlighted = NULL;
		_state = Menu::State::WAITING;
		return;  
	}  

	_selectedItem = _curHighlighted;  
	_curHighlighted->activate();  
	_curHighlighted = NULL;  

	_state = Menu::State::WAITING;
	this->release();
}  

void UIRadioButton::onTouchCancelled(Touch* touch, Event* event)
{  
	CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchCancelled] -- invalid state");
	this->retain();
	_selectedItem->selected();  
	_curHighlighted->unselected();  
	_curHighlighted = NULL;  

	_state = Menu::State::WAITING;
	this->release();
}  

void UIRadioButton::onTouchMoved(Touch* touch, Event* event)
{  
	CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchMoved] -- invalid state");
	MenuItem *curSelection = this->getItemForTouch(touch);
	if (curSelection != _curHighlighted && curSelection != NULL) 
	{  
		_curHighlighted->unselected();  
		curSelection->selected();  
		_curHighlighted = curSelection;  
		return;  
	}  
}  

