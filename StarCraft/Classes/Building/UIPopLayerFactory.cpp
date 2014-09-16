#include "UIPopLayerFactory.h"
#include "UIInfoDialog.h"
#include "UIUpgradeDialog.h"
#include "UICollector.h"
#include "UIResearchDialog.h"
#include "UIRecruitLayer.h"
#include "UIHeroPub.h"
#include "UIHeroConfiguration.h"
#include "UIWall.h"
UIPopupLayer * UIPopLayerFactory::createLayerFromTypeAndDB(UIButtonFactory::UIButtonType type,DBuilding * db,Node * node)
{
	UIPopupLayer * result = nullptr;
	switch (type)
	{
		case UIButtonFactory::UIButtonType::INFO:
			result = UIInfoDlg::CreateFromDB(db);
			break;
		case UIButtonFactory::UIButtonType::UPGRADE:
			result = UIUpgradeDialog::CreateFromDB(db);
			break;
		case UIButtonFactory::UIButtonType::UPGRADECANCLE:
			break;
		case UIButtonFactory::UIButtonType::ACCELERATE:
			break;
		case UIButtonFactory::UIButtonType::COLLECT:
			{   
				auto collect = dynamic_cast<UICollector*>(db->getBuilding());
			    collect->onCollect();
			}
			break;
		case UIButtonFactory::UIButtonType::ENTER:
			result = UIResearchDialog::CreateFromDB(db);
			break;
		case UIButtonFactory::UIButtonType::RECRUIT:
			result = UIRecruitLayer::createFromDB(db);
			break;
		case UIButtonFactory::UIButtonType::HEROCFG:
			result = UIHeroConfiguration::CreateFromDB(db);
			break;
		case UIButtonFactory::UIButtonType::HERORECRUIT:
			result = UIHeroPub::CreateFromDB(db);
			break;
		case UIButtonFactory::UIButtonType::SELECTROW:
			{
			   auto wall = dynamic_cast<UIWall*>(db->getBuilding());
			   if (wall->getRowStatus() == UIWall::RowSelectStatus::NOMAL)
			   {
				   wall->getCurrntRow();
			   }
			   else if (wall->getRowStatus() == UIWall::RowSelectStatus::ROWSELECTE)
			   {
				   wall->RotateRow();
			   }
			   if (node)
			   {
				   auto btn = dynamic_cast<UITouchButton*>(node);
				   auto label = dynamic_cast<Label*>(btn->getChildByTag(111));
				   label->setString(Configuration::getInstance()->getValue("btn_rotaterow").asString());
			   }
			}
			break;
		case UIButtonFactory::UIButtonType::ROTATEROW:
			break;;
		default:
			break;
	}
	return result;
}
