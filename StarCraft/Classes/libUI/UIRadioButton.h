#ifndef __UIRADIOBUTTON_H__
#define __UIRADIOBUTTON_H__

#include "cocos2d.h"

using namespace cocos2d;


class UIRadioButton : public Menu{  
public:  
	UIRadioButton():Menu(){}
	static UIRadioButton* create(MenuItem* item, ...);
	static UIRadioButton* createWithArray(const Vector<MenuItem*>& arrayOfItems);
	void setSelectedItem(MenuItem *item);
	void setSelectedIndex(int index);

	//virtual bool ccTouchBegan(CCTouch *pTouch,CCEvent *pEvent);
	//virtual void ccTouchEnded(CCTouch *pTouch,CCEvent *pEvent);
	//virtual void ccTouchCancelled(CCTouch *pTouch,CCEvent *pEvent);
	//virtual void ccTouchMoved(CCTouch *pTouch,CCEvent *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	virtual void onTouchCancelled(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	

private:
	MenuItem *_curHighlighted;
	Vector<MenuItem*> _menuItems; //TOD or Vector<MenuItem*> _menuItems;
};  

#endif //__UIRADIOBUTTON_H__