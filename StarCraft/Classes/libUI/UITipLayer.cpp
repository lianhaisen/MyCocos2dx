#include "UITipLayer.h"
#define CON Configuration::getInstance()

UITipLayer::UITipLayer()
{
	_closeCallBack = nullptr; 
	_Backbtn = nullptr;
	_Bg = nullptr;
}
UITipLayer::~UITipLayer()
{
	m_TopBeginNodeList.clear();
	m_TopBeginNodeList2.clear();
	m_TopBeginNodeList3.clear();
}
bool UITipLayer::init()
{
	if (UIMsgLayer::init())
	{   
		//添加背景
		this->setCloseTarget(CC_CALLBACK_0(UITipLayer::onClose,this));
		initwith();
		return true;
	}


	return false;
}

void UITipLayer::onClose()
{
	if(_closeCallBack)
	{
		_closeCallBack(this);
	}
	this->removeFromParent();
}
void UITipLayer::setVisibleBackBtn(bool b)
{
	_Backbtn->setVisible(b);
}
void UITipLayer::initwith()
{
	auto LayerTitle = this->getContentTitle();
	_Backbtn = UITouchButton::create("ui/back.png");
	LayerTitle->addChild(_Backbtn);
	_Backbtn->setPosition(_Backbtn->getContentSize().width/2,LayerTitle->getContentSize().height/2);
	_Backbtn->setClickTarget(CC_CALLBACK_0(UITipLayer::onClose,this));
}


void UITipLayer::setColseCallBack(const ccMenuCallback& callback)
{
	_closeCallBack = callback;
}


void UITipLayer::PushTopBeginNode(TopBegin nType,Node* node,Point Pos,string str,Color3B color)
{
	vector<Node*>* TmpList;
	switch(nType){
		case TopBegin::TopBeginNode :
			{
				TmpList = &m_TopBeginNodeList;
				break;
			}
		case TopBegin::TopBeginNode2 :
			{
				TmpList = &m_TopBeginNodeList2;
				break;
			}
		case TopBegin::TopBeginNode3 :
			{
				TmpList = &m_TopBeginNodeList3;
				break;
			}
		default:
			{
				return ;
			}
	}
	if(TmpList->size() >=1)
	{
		auto endPos = TmpList->at(TmpList->size()-1)->getPosition();
		Pos = Point(endPos.x,endPos.y-node->getContentSize().height-5);
	}
	node->setAnchorPoint(Point(0,0));
	auto TmpNode = Node::create();
	TmpNode->addChild(node);
	TmpNode->setContentSize(node->getContentSize());
	TmpNode->setPosition(Pos);
	getContentInfoNode()->addChild(TmpNode);
	auto lbNum = Label::createWithSystemFont(str,"",24);
	lbNum->setAnchorPoint(Point(0,0));
	lbNum->setPosition(TmpNode->getContentSize().width+5,0);
	lbNum->setTag(1);
	lbNum->setColor(color);
	TmpNode->addChild(lbNum);

	TmpList->push_back(TmpNode);


}
void UITipLayer::setTopBegin1String(TopBegin nType,int Index,const string& str,int tag,Color3B color)
{
	Label* lbNode = nullptr;
	Sprite* sp = nullptr;
	vector<Node*>* TmpList;
	switch(nType){
	case TopBegin::TopBeginNode :
		{
			TmpList = &m_TopBeginNodeList;
			break;
		}
	case TopBegin::TopBeginNode2 :
		{
			TmpList = &m_TopBeginNodeList2;
			break;
		}
	case TopBegin::TopBeginNode3 :
		{
			TmpList = &m_TopBeginNodeList3;
			break;
		}
	default:
		{
			return ;
		}
	}
	if((int)TmpList->size() >= Index){
		if(tag<=4)
			lbNode = (Label*)TmpList->at(Index-1)->getChildByTag(tag);
		else if (tag >= 7 && tag <= 5)
			sp = (Sprite*)TmpList->at(Index-1)->getChildByTag(tag);
	}
	if(lbNode){
		lbNode->setString(str);
		lbNode->setColor(color);
	}
	else if(sp)
		sp->setTexture(str);
}

void UITipLayer::clearVectors()
{
	m_TopBeginNodeList.clear();
	m_TopBeginNodeList2.clear();
	m_TopBeginNodeList3.clear();
}


/******************英雄Tip*********************/
HeroTip::HeroTip()
{
	m_Tip = nullptr;
}

HeroTip::~HeroTip()
{

}
HeroTip* HeroTip::showHeroTip( int nHeroId )
{
	auto heroTip = new HeroTip();
	auto ve = HeroInfo::getHeroConfig();
	map<int,HeroInfo*>::iterator it = ve.find(nHeroId);
	if(it!= ve.end())
	{
		auto info = it->second;
		heroTip->m_Tip = UITipLayer::create();
		heroTip->UpDataHeroTip(info);

		return heroTip;
	}

	return nullptr;
}

void HeroTip::UpDataHeroTip(HeroInfo* info)
{
	//添加背景
	auto tip = m_Tip;
	tip->getContentInfoNode()->removeAllChildren();
	tip->clearVectors();

	auto spBg = Scale9Sprite::create("dian9/huangban.9@2x.png");
	spBg->setContentSize(Size(tip->getContentInfoNode()->getContentSize().width/2,tip->getContentInfoNode()->getContentSize().height*0.98));
	spBg->setAnchorPoint(Point(0,0));
	spBg->setPosition(tip->getContentInfoNode()->getContentSize().width/2,5);
	tip->getContentScrollView()->addChild(spBg,0);
	tip->setCurBgNode(spBg);
	auto img = Sprite::create(String::createWithFormat("hero/hero%02d.png",1)->getCString());
	img->setScale(0.8f);
	tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode3,img,Point(spBg->getContentSize().width/2-img->getContentSize().width/2*0.8,spBg->getContentSize().height / 2 -img->getContentSize().height / 4 ));

	vector<string> mTmpStr;
	int nLv = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(info->id)->herolevel;
	if(nLv == 0 ) nLv = 1;
	auto HeroData = HeroUpgrade::getHeroUpgrageFromId(info->id*100+nLv);
	if(HeroData){
		mTmpStr.push_back(String::createWithFormat("attack : %d",HeroData->hurt)->getCString());
		mTmpStr.push_back(String::createWithFormat("attack Hp: %d",HeroData->hp)->getCString());
		mTmpStr.push_back(String::createWithFormat("attack target : %d",info->attackspeed)->getCString());
		mTmpStr.push_back(String::createWithFormat("attack type : %d",info->attacktype)->getCString());
		mTmpStr.push_back(String::createWithFormat("move speed : %d",info->movespeed)->getCString());
		mTmpStr.push_back(String::createWithFormat("attack AOE : %d",info->AOErange)->getCString());
	}
	//信息上
	for(auto item:mTmpStr)
	{
		auto label3 = Label::createWithSystemFont(item,"",24); 
		label3->setColor(Color3B::BLACK);
		auto pos2 = Point(spBg->getPositionX()+20,spBg->getContentSize().height- label3->getContentSize().height);
		tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode2,label3,pos2);
	}
	mTmpStr.clear();

	//技能图标
	auto spSkill = Sprite::create("ui/700012.png");
	tip->getContentInfoNode()->addChild(spSkill,10);
	spSkill->setPosition(spBg->getPositionX()+spSkill->getContentSize().width/2+20,spBg->getContentSize().width/2.2);

	auto SkillInfo = HeroLevelAndSkillLevel::getHeroLevelAndSkillLevelFromId(info->id);
	mTmpStr.push_back(String::createWithFormat("%s Lv: %d/2",info->skillname.c_str(),SkillInfo->skilllevel)->getCString());
	mTmpStr.push_back(String::createWithFormat("%s","XXXXX")->getCString());
	//下信息
	for(auto item:mTmpStr)
	{
		auto label = Label::createWithSystemFont(item,"",24); 
		label->setColor(Color3B::BLACK);
		auto pos2 = Point(spSkill->getPositionX()+spSkill->getContentSize().width*0.6,spSkill->getPositionY()+spSkill->getContentSize().height/3);
		tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode,label,pos2);
	}
	mTmpStr.clear();
}


/******************************士兵Tip**********************************/

SoldierTip::SoldierTip()
{
	m_Tip = nullptr;
}

SoldierTip::~SoldierTip()
{

}

SoldierTip* SoldierTip::createSoldierTip( CSoldier* data )
{
	auto soldierTip = new SoldierTip();

	soldierTip->m_Tip = UITipLayer::create();
	soldierTip->UpDataHeroTip(data);

	return soldierTip;

}


void SoldierTip::UpDataHeroTip( CSoldier* data )
{
	auto tip = m_Tip;
	tip->getContentInfoNode()->removeAllChildren();
	tip->clearVectors();

	auto strtitle = String::createWithFormat(CON->getValue("des").asString().c_str(),data->name.c_str(),data->id % 100);
	tip->setTitle(strtitle->getCString());

	//背景
	auto spBg = Scale9Sprite::create("dian9/huangban.9@2x.png");
	spBg->setContentSize(Size(tip->getContentInfoNode()->getContentSize().width/2,tip->getContentInfoNode()->getContentSize().height*0.98));
	spBg->setAnchorPoint(Point(0,0));
	spBg->setPosition(tip->getContentInfoNode()->getContentSize().width/2,5);
	tip->getContentScrollView()->addChild(spBg,0);
	tip->setCurBgNode(spBg);

	auto img = Sprite::create(String::createWithFormat("ui/%d.png",data->id /100)->getCString());
	tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode3,img,Point(5,tip->getContentInfoNode()->getContentSize().height / 2));

	vector<string> mTmpStr;
	mTmpStr.push_back(String::createWithFormat("attack :%d",data->attack)->getCString());
	mTmpStr.push_back(String::createWithFormat("hd : %d",data->max_hp)->getCString());
	//信息上
	for(auto item:mTmpStr)
	{
		auto label3 = Label::createWithSystemFont(item,"",24); 
		label3->setColor(Color3B::BLACK);
		auto pos1 = Point(tip->getConTentNode()->getContentSize().width/2+30,spBg->getContentSize().height- label3->getContentSize().height*1.1);
		tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode,label3,pos1);
	}
	mTmpStr.clear();

	//信息下
	mTmpStr.push_back(String::createWithFormat("attack target: %d",data->attack_target)->getCString());
	mTmpStr.push_back(String::createWithFormat("attack type: %d",data->attack_type)->getCString());
	mTmpStr.push_back(String::createWithFormat("population : %d",data->population)->getCString());
	mTmpStr.push_back(String::createWithFormat(CON->getValue("traintime").asString().c_str(),data->train_time)->getCString());
	mTmpStr.push_back(String::createWithFormat("move speed : %d",data->move_speed)->getCString());

	for(auto item:mTmpStr)
	{
		auto label3 = Label::createWithSystemFont(item,"",24); 
		label3->setColor(Color3B::BLACK);
		auto pos2 =  Point(tip->getConTentNode()->getContentSize().width/2+30,tip->getContentInfoNode()->getContentSize().height/3);
		tip->PushTopBeginNode(UITipLayer::TopBegin::TopBeginNode2,label3,pos2);
	}
	mTmpStr.clear();
}
