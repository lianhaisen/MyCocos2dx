#include "UIContainer.h"
#include "BuildingConfig.h"
#include "BuildingData.h"

UIContainer::UIContainer()
{
	_fulllabel = nullptr;
}

UIContainer::~UIContainer()
{
	CC_SAFE_RELEASE(_fulllabel);
}

void UIContainer::setBuidingData( DBuilding * db )
{
	Building::setBuidingData(db);
	if (db)
	{   
		setFullLabel(Sprite::create("building/fulllabel.png"));
		getFullLabel()->setAnchorPoint(Point(0.5,0));
		getFullLabel()->setPosition(this->getContentSize().width / 2,this->getContentSize().height);
		addChild(getFullLabel());
		if (db->curpro < db->getCurBindCfg()->storage)
		{
			getFullLabel()->setVisible(false);
		}
		
	}
}

void UIContainer::setShowFullLabel( bool show )
{
	if (getFullLabel())
	{
		getFullLabel()->setVisible(show);
	}
}

void UIContainer::updateStatus()
{   
	auto data = getBuidingData();
	if (data && getBuildingArmature())
	{   
		string str;
		if (data->curpro < data->getCurBindCfg()->storage / 2)
		{
			str = String::createWithFormat("%02d_nor",getLevel())->getCString();
			setShowFullLabel(false);
		}
		else if (data->curpro >= data->getCurBindCfg()->storage)
		{
			str = String::createWithFormat("%02d_full",getLevel())->getCString();
			setShowFullLabel(false);
		}
		else
		{
			str = String::createWithFormat("%02d_half",getLevel())->getCString();
			setShowFullLabel(true);
		}
		getBuildingArmature()->getAnimation()->play(str,-1,1);
	}
}
