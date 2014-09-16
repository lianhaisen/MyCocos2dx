#include "UIUserInfoLayer.h"
#include "AppMacros.h"
#include "UITouchButton.h"


UIUserInfoLayer* UIUserInfoLayer::create(const string &BgFile,float iScaleX, float iScaleY, string HeadFile, string ExpFile)
{
		UIUserInfoLayer *pRet = new UIUserInfoLayer();
		if(pRet && pRet->init(BgFile,iScaleX,iScaleY,HeadFile,ExpFile))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
}

bool UIUserInfoLayer::init(const string &BgFile,float iScaleX, float iScaleY, string &HeadFile,string &ExpFile)
{
	if ( !Layer::init() )
    {
        return false;
    }
	this->m_nCurNum = 0;
	this->m_nAddNum = 0; //增长值
	this->m_nMaxNum = 10000000;	//上限值
	this->m_nSpeed = 0;
	this->m_bIsAction = false; //默认 头像不动 
	m_fScaleX = iScaleX;
	m_fScaleY = iScaleY;
	createOneItme(BgFile,iScaleX,iScaleY,HeadFile,ExpFile);
	return true;
}


//更新玩家经验值 
void UIUserInfoLayer::updataHerditemLayer(float delta)
{
	int nAddNum = 1;
	if(this->m_nAddNum > this->m_nMaxNum){
		this->m_nAddNum = this->m_nMaxNum;
	}
	//增长值大小
	int itmpNum = this->m_nAddNum-this->m_nCurNum;
	//大于20或者 小于-20 都只递增1 
	if(itmpNum < -20)
		this->m_nSpeed = itmpNum/15-1;
	else if (itmpNum < 0 && itmpNum >-20)
		this->m_nSpeed = -1;
	else if(itmpNum < 20 && itmpNum > 0)
		this->m_nSpeed = 1;
	else 
		this->m_nSpeed = itmpNum/15+1;

	if(this->m_nSpeed <= 0 && this->m_nSpeed > -2)
		this->m_nSpeed = -2;
	//增加值
	this->m_nCurNum += nAddNum * this->m_nSpeed;

	if(this->m_nSpeed > 0 && this->m_nCurNum > this->m_nAddNum )
		this->m_nCurNum = this->m_nAddNum;
	else if(this->m_nCurNum <= 0)
		this->m_nCurNum = 0;
	else if(m_nCurNum < m_nAddNum && m_nSpeed < 0 ) 
		this->m_nCurNum = this->m_nAddNum;

	auto nCurNum = this->m_nCurNum;
	auto loadProgress = this->getExpLayer();
	if(loadProgress->isVisible()){
		loadProgress->setPercentage((float)nCurNum/this->m_nMaxNum*100);
	}

	char string[15] = {0};
    sprintf(string, "%d", nCurNum);
	auto label3 = this->getInfoString();
	if(label3->isVisible()){
		label3->setString(string);
	}

	if (nCurNum <= 0 ||(nCurNum >= this->m_nAddNum && this->m_nSpeed > 0 ) || nCurNum >= this->m_nMaxNum || (m_nCurNum <= m_nAddNum && m_nSpeed < 0 )  )
	{
		this->m_nSpeed =1;
		this->unschedule(schedule_selector(UIUserInfoLayer::updataHerditemLayer));
		this->m_nAddNum = this->m_nCurNum;
	}
}

//创建图标
void  UIUserInfoLayer::createOneItme(const string &BgFile,float iScaleX, float iScaleY, string &HeadFile,string &ExpFile)
{
	
    auto origin = Director::getInstance()->getVisibleOrigin();
	auto layer = this;
	//长条框
	auto sprite2 = Sprite::create(BgFile);//("ui/processbg.png");
	layer->setContentSize(Size(sprite2->getContentSize().width*iScaleX,sprite2->getContentSize().height*iScaleY));
	sprite2->setPosition(Point(layer->getContentSize()/ 2) + origin);
	sprite2->setScaleX(iScaleX);
	sprite2->setScaleY(iScaleY);
	sprite2->setTag(1);
	layer->addChild(sprite2);

	//头像图片
	if(HeadFile != " ")
	{
		auto spriteHerd = Sprite::create(HeadFile);
		spriteHerd->setAnchorPoint(Point(0.5,0.5));
		spriteHerd->setPosition(Point(-spriteHerd->getContentSize().width*0.45,layer->getContentSize().height/2) + origin);
		spriteHerd->setTag(2);
		layer->addChild(spriteHerd,10);
	}
	int FONT_SIZE =  24;
	//名字
	auto lbName = Label::createWithSystemFont("1234","",FONT_SIZE);
	lbName->setAnchorPoint(Point(0.5,0));
	lbName->setPosition(Point(layer->getContentSize().width/2,layer->getContentSize().height) + origin);
	lbName->setTag(3);
	layer->addChild(lbName);

	//内容 process
	//auto lbInfo= Label::createWithBMFont("ui/bitmapFontTest2.fnt", "10");
	auto lbInfo= Label::createWithCharMap("ui/_num.png",33,54, '0');
	lbInfo->setAnchorPoint(Point(0.5,0.5));
	lbInfo->setPosition(Point(layer->getContentSize().width/2,layer->getContentSize().height/2) + origin);
	lbInfo->setTag(4);
	lbInfo->setScale((sprite2->getContentSize().height*iScaleY )/ 54 -0.1f);
	layer->addChild(lbInfo,8);
	//lbInfo->setVisible(false);


	//进度条
	if(ExpFile == " ")
	{
		ExpFile = "ui/processblue.png";
	}
	auto loadProgress = ProgressTimer::create(Sprite::create(ExpFile));//创建一个进程条 "ui/process.png"
	loadProgress->setAnchorPoint(Point(0,0.5));
	loadProgress->setBarChangeRate(Point(1,0));//设置进程条的变化速率
	loadProgress->setType(ProgressTimer::Type::BAR);//设置进程条的类型
	loadProgress->setMidpoint(Point(0,0));//设置进度的运动方向
	loadProgress->setPosition(Point(1.5,layer->getContentSize().height/2) + origin);
	loadProgress->setPercentage((float)m_nCurNum);//设置初始值为0
	layer->addChild(loadProgress,1);
	loadProgress->setScaleX(iScaleX*0.995);
	loadProgress->setScaleY(iScaleY*0.9);
	loadProgress->setTag(5);
	//loadProgress->setVisible(false);

}

void UIUserInfoLayer::setAddNum(int Num)
{
	this->m_nAddNum += Num;
	auto imgHerd =  this->getHerdImg();
	if(imgHerd && imgHerd->isVisible() && this->m_bIsAction)
	{
		 auto scaleBy = Sequence::create(ScaleTo::create(0.15f, 1.2f),
										 ScaleTo::create(0.15f, 0.7f),
                                         ScaleTo::create(0.15f, 1),nullptr);
		 auto Ani = EaseBounceOut::create(scaleBy);
		 imgHerd->runAction(Ani);
	}
	if (!isScheduled(schedule_selector(UIUserInfoLayer::updataHerditemLayer))){
		this->schedule(schedule_selector(UIUserInfoLayer::updataHerditemLayer));
	}
}

void UIUserInfoLayer::setAddNumNoAction(int Num)
{
	m_nCurNum = Num;
	m_nAddNum = Num;

	auto loadProgress = this->getExpLayer();
	if(loadProgress->isVisible()){
		loadProgress->setPercentage((float)Num/this->m_nMaxNum*100);
	}

	char string[15] = {0};
    sprintf(string, "%d", Num);
	auto label3 = this->getInfoString();
	if(label3->isVisible()){
		label3->setString(string);
	}
}

void UIUserInfoLayer::setName(const string& text)
{
	auto lbName = this->getNameLayer();
	lbName->setString(text);
}

void UIUserInfoLayer::setHerdImg(const string &filename)
{
	auto spHerdImg = this->getHerdImg();
	spHerdImg->setTexture(filename);
}
//设置长条
//void UIUserInfoLayer::setBg(const string &filename)
//{
//	auto spBg = this->getBg();
//	spBg->setTexture(filename);
//}

void UIUserInfoLayer::setMaxNum(int nMaxNum,const string& text)
{
	m_nMaxNum = nMaxNum;
	if(text != " ")
	{
		this->setName(text);
	}
	auto loadProgress = this->getExpLayer();
	int nCurNum = m_nCurNum;
	if(nCurNum > m_nMaxNum )
	{
		nCurNum = m_nMaxNum;
	}
	if(loadProgress->isVisible()){
		loadProgress->setPercentage((float)nCurNum/m_nMaxNum*100);
	}

}

void UIUserInfoLayer::setinitNum(int nCurNum,int nMaxNum,const string& text)
{
	m_nCurNum = nCurNum;
	m_nAddNum = nCurNum;
	m_nMaxNum = nMaxNum;
	if(m_nCurNum > m_nMaxNum)
	{
		m_nCurNum = m_nMaxNum;
		m_nAddNum = m_nMaxNum;
	}
	if(text != " ")
	{
		this->setName(text);
	}
	auto loadProgress = this->getExpLayer();
	if(loadProgress->isVisible()){
		loadProgress->setPercentage((float)m_nCurNum/this->m_nMaxNum*100);
	}

	char string[15] = {0};
    sprintf(string, "%d", m_nCurNum);
	auto label3 = this->getInfoString();
	if(label3->isVisible()){
		label3->setString(string);
	}
}
void UIUserInfoLayer::setHeadAction(bool b)
{
	m_bIsAction = b;
}