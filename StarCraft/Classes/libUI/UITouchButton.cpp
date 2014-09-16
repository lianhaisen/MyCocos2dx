#include "UITouchButton.h"
#include "UIGraySprite.h"

UITouchButton::UITouchButton(void)
{
	m_sprite = NULL;
    m_bIsTouchEnabled = true;
    m_bIsLongTouched = false;
    m_bIsMaybeLongTouched = false;
    m_bIsUsedForScroller = false;
    m_bIsMoveLiveTooFar = false;
    m_bIsNeedLongTouch = false;
    m_lTouchBeganTime = 0;
	_clickcallback = nullptr;
	_longcallback = nullptr;
	_listen = nullptr;
}

UITouchButton::~UITouchButton(void)
{
	this->unscheduleUpdate();
}

UITouchButton* UITouchButton::create(const char* pszFileName)
{
		UITouchButton *pRet = new UITouchButton();
		if(pRet && pRet->initWithFile(pszFileName))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
}

bool UITouchButton::initWithFile(const char *pszFilename)
{
	    bool bRet = false;
        do {    
			    init();
			    m_sprite = UIGraySprite::create(pszFilename);
				setContentSize(m_sprite->getContentSize());
                //添加图片
                addChild(m_sprite);
                scheduleUpdate();
				
                bRet = true;
        } while (0);
        return bRet;
}

void UITouchButton::onEnter()
{
	 Node::onEnter();

}

void UITouchButton::onExit()
{
    Node::onExit();
}

void UITouchButton::update(float dt)
{
	if (m_bIsNeedLongTouch && m_bIsMaybeLongTouched)
	{
        //大于2秒算长按
		if (_getMillisecondTime() - m_lTouchBeganTime >= 1.5f){
				m_bIsMaybeLongTouched = false;
				m_bIsLongTouched = true;
				 
				//执行动画
				ScaleTo* stBig = ScaleTo::create(0.1f, 0.9f);
				ScaleTo* stNormal = ScaleTo::create(0.1f, 1.1f);
				RepeatForever * repeat = RepeatForever::create(Sequence::create(stBig,stNormal, NULL));  
				runAction(repeat);
		}
   }
   
   if( dt > 1.0f / 55 )
	  return;
   
   if (m_bIsNeedLongTouch && m_bIsLongTouched)
   {
	   _doLongClickEvent();
   }

   
}

bool UITouchButton::_isTouchInSprite(Touch *pTouch)
{
        Point p = convertTouchToNodeSpaceAR(pTouch);
        Size s = m_sprite->getContentSize();
        Rect r = Rect(-s.width/2, -s.height/2, s.width, s.height);
        return r.containsPoint(p);
}

bool UITouchButton::onTouchesBegan(Touch * pTouch, cocos2d::Event  *event)
{
        if (isVisible() && m_bIsTouchEnabled && _isTouchInSprite(pTouch))
		{
                m_bIsMaybeLongTouched = true;
                m_bIsMoveLiveTooFar = false;
                mTouchBeganPoint = pTouch->getLocation();
                m_lTouchBeganTime = _getMillisecondTime();
               
				ScaleTo* smallac = ScaleTo::create(0.05f, 0.9f);
				runAction(smallac);

                return true;
        }
        
        return false;
}

void UITouchButton::onTouchesMoved(Touch * pTouch, cocos2d::Event  *event){
        
	    if (m_bIsMaybeLongTouched && !m_bIsLongTouched)
		{
                if (!_isTouchInSprite(pTouch)){
                        m_bIsMaybeLongTouched = false;
                }
        }
}

void UITouchButton::onTouchesEnded(Touch * pTouch, cocos2d::Event  *event)
{
        //长按事件的点击事件和普通点击事件不是同一个
        if (!m_bIsLongTouched)
		{
			if (_isTouchInSprite(pTouch))
			{    
				auto nomal = Sequence::createWithTwoActions( ScaleTo::create(0.05f, 1.0),
					                                         CallFunc::create(CC_CALLBACK_0(UITouchButton::_doSingleClickEvent,this)));
			    runAction(nomal);
			}   
			else if (getScale() != 1.0f)
			{   

				runAction(ScaleTo::create(0.05f, 1.0));
			}
			

			
        }
		else
		{
			stopAllActions();
			ScaleTo* normal = ScaleTo::create(0.05f, 1.0);
			runAction(normal);
		}


        m_bIsTouchEnabled = true;
        m_bIsLongTouched = false;
        m_bIsMaybeLongTouched = false;
        m_lTouchBeganTime = 0;
		//执行还原动画
	
}

void UITouchButton::onTouchCancelled(Touch * touch, cocos2d::Event  *event)
{
        //onTouchesEnded(pTouch, pEvent);
}


void UITouchButton::_doLongClickEvent()
{
       if (_longcallback)
       {
		   _longcallback(this);
       }
}

void UITouchButton::_doSingleClickEvent()
{
      if (_clickcallback)
      {
		  _clickcallback(this);
      }
}

float UITouchButton::_getMillisecondTime()
{       

        struct timeval now;
        gettimeofday(&now, NULL); 
        int temp = (int)(now.tv_sec * 1000 + now.tv_usec / 1000);
        return temp/1000.f;
}

void UITouchButton::setOpacity(GLubyte opacity)
{
        m_sprite->setOpacity(opacity);
}

void UITouchButton::setEnabled(bool v)
{
        m_bIsTouchEnabled = v;
}

bool UITouchButton::isEnabled()
{
        return m_bIsTouchEnabled;
}

void UITouchButton::setIsUsedForScroller(bool v)
{
        m_bIsUsedForScroller = v;
		if (_listen)
		{
			_listen->setSwallowTouches(!v);
		}

}

void UITouchButton::setIsNeedLongTouch(bool v)
{
        m_bIsNeedLongTouch = v;
}

void UITouchButton::setGray(bool gray)
{
	if (m_sprite)
	{
		m_sprite->setGray(gray);
	}
}


bool UITouchButton::init()
{
	if (Node::init())
	{   
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(UITouchButton::onTouchesBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(UITouchButton::onTouchesMoved, this);
		listener->onTouchEnded = CC_CALLBACK_2(UITouchButton::onTouchesEnded, this);
		listener->onTouchCancelled = CC_CALLBACK_2(UITouchButton::onTouchCancelled,this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		_listen = listener;
	
 		return true;
	}
	return false;
}

void UITouchButton::setClickTarget( const ccMenuCallback& callback )
{
	_clickcallback = callback;
}

void UITouchButton::setLongPressTarget( const ccMenuCallback& callback )
{
	_longcallback = callback;
}
