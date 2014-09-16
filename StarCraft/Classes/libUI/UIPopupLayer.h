//
//  PopupLayer.h
//  TestCpp
//
//  Created by leafsoar on 7/29/13.
//
//

#ifndef TestCpp_PopupLayer_h
#define TestCpp_PopupLayer_h

#include "cocos2d.h"
using namespace cocos2d;
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;


class UIPopupLayer: public LayerColor{
public:
    UIPopupLayer();
    ~UIPopupLayer();
    
    virtual bool init();
    CREATE_FUNC(UIPopupLayer);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event); 

    void setTitle(const char* title, int fontsize = 20);
    void setContentText(const char* text, int fontsize = 20, int padding = 50, int paddintTop = 100);
	//初始化背景
	void initBackGround();

   
	virtual void onEnter();
    virtual void onExit();
    virtual void dlgClose();

	void setCloseTarget( const ccMenuCallback& callback);

private:

    void buttonCallback(Ref* pSender);

    // 文字内容两边的空白区
    int m_contentPadding;
    int m_contentPaddingTop;

	CC_SYNTHESIZE_RETAIN(Sprite*,_infoPic,InfoPic);
    CC_SYNTHESIZE_RETAIN(Label*, m__ltTitle, LabelTitle);
    CC_SYNTHESIZE_RETAIN(Label*, m__ltContentText, LabelContentText);
	CC_SYNTHESIZE_RETAIN(Sprite*,m__sfCloseButton,CloseButton);
    CC_SYNTHESIZE_RETAIN(Node*,m__contextNode,ContextNode)
	CC_SYNTHESIZE_RETAIN(Node*,_timerNode,TimerNode);
	
	void dlgCloseAction();
	bool checkIsInNode(Node * node,Touch * pTouch);

protected:
    ccMenuCallback _closeCallBack;
    
};

#endif
