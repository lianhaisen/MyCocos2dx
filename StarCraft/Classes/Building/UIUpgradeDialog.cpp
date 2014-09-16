#include "UIUpgradeDialog.h"
#include "BuildingCommon.h"
#include "BuildingConfig.h"
#include "BuildingData.h"
#include "UITouchButton.h"
#include "UIBuildingBase.h"

UIUpgradeDialog::UIUpgradeDialog()
{
	_db = nullptr;
}
UIUpgradeDialog::~UIUpgradeDialog()
{

}

UIUpgradeDialog* UIUpgradeDialog::CreateFromDB( DBuilding * db )
{
	auto Ret = UIUpgradeDialog::create();
	Ret->_db = db;
	Ret->setInfoPic(Sprite::create(BuildingCommon::getPicFromId(db->type)));
	Ret->getInfoPic()->setScale(0.5f);

	auto cfg = db->getCurBindCfg();
	auto nextcfg = db->getNextBindCfg();
	if (cfg  && nextcfg )
	{   
		auto str = String::createWithFormat(Configuration::getInstance()->getValue("upgrade").asString().c_str(), nextcfg->id % 100);
		Ret->setTitle(str->getCString());
	}
    
	Ret->createTimer();
	return Ret;
}

void UIUpgradeDialog::createTimer()
{
	setTimerNode(Node::create());
	
	auto con = Configuration::getInstance();
	auto curcfg =  _db->getCurBindCfg();
	auto nextcfg = _db->getNextBindCfg();
	auto maxcfg =  _db->getMaxBindCfg();

	if (_db->type / 100 == 740007)
	{
		//军营
		auto populationadd = String::createWithFormat(con->getValue("populationadd").asString().c_str(),curcfg->population ,nextcfg->population - curcfg->population);
		createTimer(populationadd->getCString(),curcfg->population ,nextcfg->population,maxcfg->population);
	}
	else
	{   
		auto strstorageadd = String::createWithFormat(con->getValue("storageadd").asString().c_str(),curcfg->storage ,nextcfg->storage - curcfg->storage);
		createTimer(strstorageadd->getCString(),curcfg->storage ,nextcfg->storage,maxcfg->storage);
		
	}

	//血量增加
	auto hpadd = String::createWithFormat(con->getValue("hpadd").asString().c_str(),curcfg->max_hp ,nextcfg->max_hp - curcfg->max_hp);
	createTimer(hpadd->getCString(),curcfg->max_hp ,nextcfg->max_hp,maxcfg->max_hp,-50.0f);


	//时间标签
	auto str = String::createWithFormat(con->getValue("upgradetime").asString().c_str(),nextcfg->cost_time);
	auto timelabel = Label::createWithSystemFont(str->getCString(),"",10);
	timelabel->setColor(Color3B::BLACK);
	timelabel->setHorizontalAlignment(TextHAlignment::LEFT);
	timelabel->setVerticalAlignment(TextVAlignment::BOTTOM);
	timelabel->setPosition(Point( 0, -100.0f));
	getTimerNode()->addChild(timelabel);


	if (_db->status != DBuilding::BuildingStatus::UPGRADE || _db->status != DBuilding::BuildingStatus::NewBuild)
	{
		auto ptn = UITouchButton::create("ui/button.png");
		auto str = String::createWithFormat("%d",nextcfg->cost_value);
		auto label = Label::createWithSystemFont(str->getCString(),"",20);
		ptn->addChild(label);
		ptn->setPosition(Point(getContextNode()->getContentSize().width / 2,100));
		ptn->setClickTarget(CC_CALLBACK_1(UIUpgradeDialog::onUpgrade,this));
		getContextNode()->addChild(ptn,10);
	}

}

void UIUpgradeDialog::createTimer( string str,int cur,int next,int max,float offsetY /*= 0*/ )
{
	auto bg = Sprite::create("ui/processbg2.png");
	bg->setScale(3.0f);

	auto timer = ProgressTimer::create( Sprite::create("ui/process.png"));
	timer->setScale(3.0f);
	timer->setType(ProgressTimer::Type::BAR);
	timer->setMidpoint(Point(0,100));
	timer->setBarChangeRate(Point(1,0));
	timer->setPercentage( (float)cur /max  * 100);

	auto timer2 =  ProgressTimer::create( Sprite::create("ui/processbg3.png"));
	timer2->setScale(3.0f);
	timer2->setType(ProgressTimer::Type::BAR);
	timer2->setMidpoint(Point(0,100));
	timer2->setBarChangeRate(Point(1,0));
	timer2->setPercentage((float)next / max * 100);
	
	auto label = Label::createWithSystemFont(str,"",10);
	label->setColor(Color3B::BLACK);
	label->setHorizontalAlignment(TextHAlignment::LEFT);
	label->setVerticalAlignment(TextVAlignment::BOTTOM);
	label->setPosition(Point( 0, bg->getContentSize().height + 10 + offsetY));

	bg->setPositionY(offsetY);
	timer2->setPositionY(offsetY);
	timer->setPositionY(offsetY);

	getTimerNode()->addChild(bg);
	getTimerNode()->addChild(timer2);
	getTimerNode()->addChild(timer);
	getTimerNode()->addChild(label);
}

void UIUpgradeDialog::onUpgrade( Ref* ref )
{ 
	//TODO 判定条件 扣资源什么的~
	if (_db != nullptr && _db->setUpgrade())
	{    
		if (_db->getBuilding()!= nullptr)
		{
			_db->getBuilding()->updateUpgradeStatus(); 
		}
	}

	auto se = Sequence::createWithTwoActions(DelayTime::create(0.3f),
		CallFunc::create(CC_CALLBACK_0(UIUpgradeDialog::onClose,this)));
	this->runAction(se);
}

void UIUpgradeDialog::onClose()
{
	if (_closeCallBack)
	{
		_closeCallBack(this);
	}
}


