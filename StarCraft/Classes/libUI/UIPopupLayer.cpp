//
//  PopupLayer.cpp
//  TestCpp
//
//  Created by leafsoar on 7/29/13.
//
//

#include "UIPopupLayer.h"

#define BoxSize Size(544,520)

UIPopupLayer::UIPopupLayer():
  m_contentPadding(0)
, m_contentPaddingTop(0)
, m__ltContentText(nullptr)
, m__ltTitle(nullptr)
, m__contextNode(nullptr)
, m__sfCloseButton(nullptr)
, _closeCallBack(nullptr)
,_infoPic(nullptr)
,_timerNode(nullptr)
{
    
}

UIPopupLayer::~UIPopupLayer(){

    CC_SAFE_RELEASE(m__ltContentText);
    CC_SAFE_RELEASE(m__ltTitle);
	CC_SAFE_RELEASE(m__sfCloseButton);
	CC_SAFE_RELEASE(m__contextNode);
	CC_SAFE_RELEASE(_timerNode);
}

bool UIPopupLayer::init(){
    bool bRef = false;
    do{
        CC_BREAK_IF(!LayerColor::initWithColor(Color4B(25, 25, 25, 125)));
        
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan =  CC_CALLBACK_2(UIPopupLayer::onTouchBegan, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		setContextNode(Node::create());
		getContextNode()->setContentSize(BoxSize);
		setCloseButton(Sprite::create("ui/close.png"));

        bRef = true;
    }while(0);
    return bRef;
}


void UIPopupLayer::setTitle(const char *title, int fontsize){
   
	if (getLabelTitle() == nullptr)
	{
		Label* ltfTitle = Label::createWithSystemFont(title, "", fontsize);
		setLabelTitle(ltfTitle);
	}
	else
	{
		getLabelTitle()->setString(title);
		getLabelTitle()->setSystemFontSize(fontsize);
	}
	

}

void UIPopupLayer::setContentText(const char *text, int fontsize, int padding, int paddingTop){
   
	Label* ltf = Label::createWithSystemFont(text, "", fontsize);
	ltf->setColor(Color3B::BLACK);
    setLabelContentText(ltf);
    m_contentPadding = padding;
    m_contentPaddingTop = paddingTop;
}


void UIPopupLayer::onEnter(){
    
	Layer::onEnter();
    
    auto winSize = Director::getInstance()->getWinSize();
    auto pCenter = Point(winSize.width / 2, winSize.height / 2);
    Size contentSize ;

    
	addChild(getContextNode());
	getContextNode()->setAnchorPoint(Point(0.5f,0.5f));
	getContextNode()->setPosition(pCenter);

	pCenter = Point(getContextNode()->getContentSize().width / 2 ,getContextNode()->getContentSize().height / 2 );
	contentSize = getContextNode()->getContentSize();
	initBackGround();

    
	getCloseButton()->setPosition(Point(getContextNode()->getContentSize().width - 30,getContextNode()->getContentSize().height - 30 ));
	getContextNode()->addChild(getCloseButton());
	
    // 显示对话框标题
    if (getLabelTitle() != nullptr){
        getLabelTitle()->setPosition(pCenter + Point(0, contentSize.height / 2 - 35.0f));
        getContextNode()->addChild(getLabelTitle());
    }

	if (getInfoPic() != nullptr)
	{   
		getInfoPic()->setAnchorPoint(Point(0,1));
		getInfoPic()->setPosition(pCenter + Point( - contentSize.width / 2  + 30.0f, contentSize.height / 2 - 100.f));
		getContextNode()->addChild(getInfoPic());
	}
	
	if (getTimerNode() != nullptr)
	{   
		//TODO 设置位置
	    getTimerNode()->setPosition(pCenter + Point(30.0f, contentSize.height / 2 - 150.f));
		getContextNode()->addChild(getTimerNode());
	}
	

    
    // 显示文本内容
    if (getLabelContentText() != nullptr){
        Label* ltf = getLabelContentText();
        ltf->setPosition(pCenter + Point(0,-30.0f));
		// TODO: setDimensions
        //ltf->setDimensions(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop);
        ltf->setHorizontalAlignment(TextHAlignment::CENTER);
        getContextNode()->addChild(ltf);
    }

    // 弹出效果
    Action* popupLayer = Sequence::create(FadeIn::create(0.06f),
                                            ScaleTo::create(0.06f, 1.1f),
                                            ScaleTo::create(0.08f, 1.0f), NULL);
    getContextNode()->runAction(popupLayer);

}

void UIPopupLayer::onExit(){
    
	
    Layer::onExit();
}

void UIPopupLayer::dlgClose()
{	
	if (_closeCallBack)
	{
		_closeCallBack(this);
	}
}

void UIPopupLayer::dlgCloseAction()
{
	Action* popupLayer = Sequence::create(ScaleTo::create(0.06f, 1.1f),
		FadeOut::create(0.08f),
		CallFunc::create(CC_CALLBACK_0(UIPopupLayer::dlgClose,this)),
		NULL);
	getContextNode()->runAction(popupLayer);
}

bool UIPopupLayer::checkIsInNode(Node * node,Touch * pTouch)
{
	auto p = node->convertTouchToNodeSpaceAR(pTouch);
	Size s = node->getContentSize();
	Rect r = Rect(-s.width/2, -s.height/2, s.width, s.height);
	return r.containsPoint(p);
}

bool UIPopupLayer::onTouchBegan( Touch *pTouch, Event *unused_event )
{
	if (!checkIsInNode(getContextNode(),pTouch) || checkIsInNode(getCloseButton(),pTouch))
	{
		dlgCloseAction();
	}
	return true;
}

void UIPopupLayer::setCloseTarget( const ccMenuCallback& callback )
{
	_closeCallBack = callback;
}

void UIPopupLayer::initBackGround()
{
	//添加背景
	auto tsize = getContextNode()->getContentSize();

	auto imgmiddle = Sprite::create("ui/1147.0.png");
	imgmiddle->setPosition(tsize.width / 2,tsize.height/2);
	imgmiddle->setScaleX(tsize.width);
	getContextNode()->addChild(imgmiddle);

	auto imgleft = Sprite::create("ui/1145.0.png");
	imgleft->setAnchorPoint(Point(0,0.5f));
	imgleft->setPosition(0 - imgleft->getContentSize().width,tsize.height/2);
	getContextNode()->addChild(imgleft);

	auto imgright = Sprite::create("ui/1145.0.png");
	imgright->setScaleX(-1);
	imgright->setAnchorPoint(Point(1,0.5f));
	imgright->setPosition(tsize.width ,tsize.height/2);
	getContextNode()->addChild(imgright);
}


