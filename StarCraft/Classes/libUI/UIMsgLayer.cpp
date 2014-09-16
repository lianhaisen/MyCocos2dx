#include "UIMsgLayer.h"
#include "AppMacros.h"

#define BoxSize Size(Director::getInstance()->getWinSize().width / 1.5,520)

UIMsgLayer::UIMsgLayer()
{
	_contentNode = nullptr;
	_contentInfoNode = nullptr;
	_contentTitle = nullptr;
	m_pTmpInfoNode = nullptr;
	m_CloseCallback = nullptr;
	m_scrollView = nullptr;
	m_LeftBtn = nullptr;
	m_RightBtn = nullptr;
	m_CurrentStart = 1;
}

UIMsgLayer::~UIMsgLayer()
{
	CC_SAFE_RELEASE(_contentNode);
	CC_SAFE_RELEASE(_contentInfoNode);
	CC_SAFE_RELEASE(_contentTitle);
	CC_SAFE_RELEASE(m_pTmpInfoNode);
}


bool UIMsgLayer::init()
{
	if (UIPopupLayer::init())
	{   
		//添加背景
		getContextNode()->setContentSize(BoxSize);
		//this->setCloseTarget(CC_CALLBACK_0(UIMsgLayer::onClose,this));
		initwithSize();

		return true;
	}
	return false;
}

void UIMsgLayer::onClose()
{
	//this->removeFromParent();
}

void UIMsgLayer::initwithSize()
{
	Size size = getContextNode()->getContentSize();
	setConTentNode(Node::create());
	getConTentNode()->setContentSize(BoxSize);
	//getConTentNode()->setPosition(BoxSize.width/4,BoxSize.height/4);
	getContextNode()->addChild(getConTentNode(),100);

	//设置添加Title的Node
	setContentTitle(Node::create());
	auto titleNode = getContentTitle();
	titleNode->setContentSize(Size(size.width,size.height*0.105));
	titleNode->setPosition(Point(0,getConTentNode()->getContentSize().height-titleNode->getContentSize().height));
	getConTentNode()->addChild(titleNode);


	auto Nodesize = Size(size.width,size.height*0.85);

	ScrollView * scrollView = ScrollView::create();
	scrollView->setViewSize(Nodesize);
	scrollView->setContentSize(Nodesize);
	scrollView->setPosition(Point(0,14.5));//(Point(size.width/2 - size.width/2,size.height/2- size.height/2));
	getConTentNode()->addChild(scrollView);
	scrollView->setTouchEnabled(false);
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
	m_scrollView = scrollView;

	//设置添加内容的Node
	setContentInfoNode(Node::create());
	auto infoNode = getContentInfoNode();
	infoNode->setContentSize(Nodesize);
	scrollView->addChild(infoNode,10);

	/*auto layer = LayerColor::create(Color4B(125,255,255,125));
	layer->setContentSize(Nodesize);
	infoNode->addChild(layer);*/
	auto btnLeft = UITouchButton::create("ui/leftbtn.png");
	btnLeft->setPosition(Point(-btnLeft->getContentSize().width,size.height/2));
	btnLeft->setClickTarget(CC_CALLBACK_1(UIMsgLayer::onClickLeftBtnCallBack,this));
	getConTentNode()->addChild(btnLeft,10,m_CurrentStart);
	m_LeftBtn = btnLeft;

	auto btnRight = UITouchButton::create("ui/leftbtn.png");
	btnRight->setPosition(Point(size.width+btnRight->getContentSize().width,size.height/2));
	btnRight->setClickTarget(CC_CALLBACK_1(UIMsgLayer::onClickRightBtnCallBack,this));
	getConTentNode()->addChild(btnRight,10,m_CurrentStart);
	btnRight->setRotation(180);
	m_RightBtn = btnRight;
	m_RightBtn->setVisible(false);
	m_LeftBtn->setVisible(false);

}

void UIMsgLayer::setVIsibelLeftRightBtn(bool bTrue)
{
	m_RightBtn->setVisible(bTrue);
	m_LeftBtn->setVisible(bTrue);
}

void UIMsgLayer::onClickLeftBtnCallBack(Ref* sender)
{
	auto item =  static_cast<MenuItem*>(sender);
	m_CurrentStart -= 1;
	if(m_CurrentStart < 1)
		m_CurrentStart = 2;
	item->setTag(m_CurrentStart);

	auto node = getContentInfoNode();
	setContentTmpInfoNode(Node::create());
	m_pTmpInfoNode->setContentSize(node->getContentSize());

	auto pos = Point(-node->getContentSize().width,node->getPositionY());
	PopInfoNodeAction(pos);

}
void UIMsgLayer::onClickRightBtnCallBack(Ref* sender)
{
	auto item =  static_cast<MenuItem*>(sender);
	m_CurrentStart += 1;
	if(m_CurrentStart > 2)
		m_CurrentStart = 1;
	item->setTag(m_CurrentStart);
	auto node = getContentInfoNode();
	setContentTmpInfoNode(Node::create());
	m_pTmpInfoNode->setContentSize(node->getContentSize());
	m_pTmpInfoNode->setPosition(Point(-node->getContentSize().width,node->getPositionY()));
	auto pos = Point(node->getContentSize().width,node->getPositionY());
	PopInfoNodeAction(pos);
}

void UIMsgLayer::onPopInfoNode()
{
	auto node = getContentInfoNode();
	node->removeFromParent();
	node = nullptr;
	setContentInfoNode(m_pTmpInfoNode);


}

void UIMsgLayer::PopInfoNodeAction(Point Pos)
{
	auto node = getContentInfoNode();

	auto popup = Spawn::create(
		MoveTo::create(0.06f, Pos),
		FadeOut::create(0.08f),
		NULL);

	node->runAction(EaseIn::create(popup,1.0f));

	auto pos2 = Point(0,node->getPositionY());
	m_pTmpInfoNode->setVisible(true);

	Action* popupLayer = Sequence::create(Spawn::create(MoveTo::create(0.06f, pos2),FadeIn::create(0.08f),NULL),
	CallFunc::create(CC_CALLBACK_0(UIMsgLayer::onPopInfoNode,this)),
	NULL);

	m_pTmpInfoNode->runAction(popupLayer);
}

//设置拖拽页数
void UIMsgLayer::setDragPage(int Page)
{
	if(getContentScrollView()){
		auto size = Size::ZERO;
		auto d = getContentScrollView()->getDirection();
		if(d == ScrollView::Direction::HORIZONTAL){
			size = Size(getContentScrollView()->getViewSize().width*Page,getContentScrollView()->getViewSize().height);
		}
		else if(d == ScrollView::Direction::VERTICAL )
			size = Size(getContentScrollView()->getViewSize().width,getContentScrollView()->getViewSize().height*Page);
		else
			size = getContentScrollView()->getViewSize();

		getContentScrollView()->setContentSize(size);
		
	}
}