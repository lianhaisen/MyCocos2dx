#include "UICollector.h"
#include "UITouchButton.h"
#include "BuildingData.h"
#include "BuildingConfig.h"
#include "BuildingFunc.h"

void UICollector::updateStatus()
{   
	auto data = getBuidingData();
	if (data && data->status == DBuilding::BuildingStatus::WORK)
	{    
		auto str = String::createWithFormat("%02d_work",getLevel())->getCString();
		getBuildingArmature()->getAnimation()->play(str,-1,1);
	}
	else
	{
		Building::updateStatus();
	}
}

UICollector::UICollector()
{
	_collectIcon = nullptr;
}

UICollector::~UICollector()
{
	CC_SAFE_RELEASE(_collectIcon);
	this->unschedule(schedule_selector(UICollector::updateWorkStatus));
}

void UICollector::setBuidingData( DBuilding * db )
{
	Building::setBuidingData(db);
	if (getCollectIcon())
	{
		getCollectIcon()->removeFromParent();
	}
	switch (db->type / 100)
	{
	case 740001:
		setCollectIcon(UITouchButton::create("building/blue_icon.png"));
		break;
	case 740003:
		setCollectIcon(UITouchButton::create("building/gold_icon.png"));
		break;
	case 740005:
		setCollectIcon(UITouchButton::create("building/black_icon.png"));
		break;
	default:
		break;
	}
	getCollectIcon()->setPosition(getContentSize().width / 2,getContentSize().height + 50);
	addChild(getCollectIcon());
	getCollectIcon()->setVisible(false);
	getCollectIcon()->setClickTarget(CC_CALLBACK_0(UICollector::onCollect,this));
	this->schedule(schedule_selector(UICollector::updateWorkStatus),1.0f);
}

void UICollector::updateWorkStatus( float dt )
{   
   
	auto db = getBuidingData();
	if (!db )
	{
		return;
	}
	if (db->status != DBuilding::BuildingStatus::UPGRADE && db->status != DBuilding::BuildingStatus::NewBuild)
	{   
		auto cfg =  db->getCurBindCfg();
		if (db->status == DBuilding::BuildingStatus::RELEX && db->curpro < cfg->storage)
		{
			db->status = DBuilding::BuildingStatus::WORK;
			db->updatePro();
			updateStatus();
		}
		if (db->curpro > 15)
		{
			getCollectIcon()->setVisible(true);
		}
	}

	if ( db->status == DBuilding::BuildingStatus::WORK)
	{   
		auto before = db->curpro;
		auto cfg = db->getCurBindCfg();
		auto houroutput = (float)cfg->output / 3600;//Ã¿Ãë²úÁ¿
		time_t tt;
		time(&tt);
		if (tt > db->timestamp)
		{
		   auto output = (int)((tt - db->timestamp) * houroutput);
		   if (output > 1)
		   {
			   db->curpro += output;
			   if (db->curpro > cfg->storage)
			   {
				   db->curpro = cfg->storage;
				   db->status = DBuilding::BuildingStatus::RELEX;
				   updateStatus();
			   }
			   db->updatePro();
		   }
		    
		}
	
	}
}

void UICollector::onCollect()
{   
	BuildingFunc::getInstance()->CollectResource(getBuidingData());
	getCollectIcon()->setVisible(false);
}
