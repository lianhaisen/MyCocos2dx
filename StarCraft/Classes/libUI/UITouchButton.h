
#ifndef __UITOUCHBUTTON__
#define __UITOUCHBUTTON__

#include "cocos2d.h"
class UIGraySprite;
using namespace cocos2d;

class UITouchButton : public Node
{
public:
	UITouchButton(void);
	virtual ~UITouchButton(void);
    static UITouchButton* create(const char* pszFileName);


public :

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float dt);

	UIGraySprite * getSprite(){return m_sprite;}

    bool onTouchesBegan(Touch * touch, cocos2d::Event  *event);
    void onTouchesMoved(Touch * touch, cocos2d::Event  *event);
    void onTouchesEnded(Touch * touch, cocos2d::Event  *event);
    void onTouchCancelled(Touch * touch, cocos2d::Event  *event);

	void setOpacity(GLubyte opacity);
    void setEnabled(bool v);
    bool isEnabled();

    void setIsUsedForScroller(bool v);

    void setIsNeedLongTouch(bool v);


	//默认不灰
	void setGray(bool gray);

	// 单击
	void setClickTarget( const ccMenuCallback& callback);
	// 长按回调对象
	void setLongPressTarget( const ccMenuCallback& callback);
	
	

private:
	UIGraySprite* m_sprite;
	EventListenerTouchOneByOne * _listen;
	bool m_bIsNeedLongTouch;
    bool m_bIsTouchEnabled;
    bool m_bIsLongTouched;
    bool m_bIsMaybeLongTouched;

    bool m_bIsUsedForScroller;
    bool m_bIsMoveLiveTooFar;

	float m_lTouchBeganTime;
    Point mTouchBeganPoint;
    
	//判断是否触摸到
    bool _isTouchInSprite(Touch *pTouch);
    //获取秒级时间
    float _getMillisecondTime(); 
	bool initWithFile(const char *pszFilename);

    void _doLongClickEvent();
    void _doSingleClickEvent();

	
protected:
	ccMenuCallback _clickcallback;
	ccMenuCallback _longcallback;

};

#endif