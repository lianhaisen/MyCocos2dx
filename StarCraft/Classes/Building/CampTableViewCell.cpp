#include "CampTableViewCell.h"

#define  BoxSize  Size(86,108) 

CampTableViewCell::CampTableViewCell()
{
	_bg = nullptr;
	_contentNode = nullptr;
}

CampTableViewCell::~CampTableViewCell()
{
	CC_SAFE_RELEASE(_bg);
	CC_SAFE_RELEASE(_contentNode);
}

bool CampTableViewCell::init()
{  
   setBg(Sprite::create("ui/629.png"));
   getBg()->setAnchorPoint(Point::ZERO);
   addChild(getBg());

   return true;
}

void CampTableViewCell::setInfo( int id,int lv,int count )
{
	disableInfo();
    setContentNode(Node::create());
	addChild(getContentNode());
	
	auto bg = Sprite::create("ui/628.png");
	bg->setAnchorPoint(Point::ZERO);
	getContentNode()->addChild(bg);


	auto str = String::createWithFormat("ui/%d.png",id);
	auto head = Sprite::create(str->getCString());
	head->setPosition(bg->getContentSize().width /2,bg->getContentSize().height/2);
	getContentNode()->addChild(head);

	str = String::createWithFormat("Lv.%d",lv);
	auto label = Label::createWithSystemFont(str->getCString(),"",15);
	label->setPosition(Point(BoxSize.width/2,30));
	getContentNode()->addChild(label);

	str = String::createWithFormat("%d",count);
	label =  Label::createWithSystemFont(str->getCString(),"",15);
	label->setPosition(Point(BoxSize.width/2,15));
	getContentNode()->addChild(label);

}

void CampTableViewCell::disableInfo()
{
	if (getContentNode())
	{
		getContentNode()->removeFromParent();
		setContentNode(nullptr);
	}
}


