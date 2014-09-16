#include "UIButtonFactory.h"
#include "UIButtonFactory.h"
#include "BuildingData.h"
#include "BuildingConfig.h"
#include "UITouchButton.h"
#include "UIWall.h"

Vector<UITouchButton*> UIButtonFactory::getButtonsFromDB( DBuilding * db ,const ccMenuCallback& callback)
{   
	Vector<UITouchButton*> vec;
	auto type = db->type / 100;
	std::vector<UIButtonType> typelist;

	if (db->status == DBuilding::BuildingStatus::UPGRADE)
	{
        typelist.push_back(UIButtonType::INFO);
		typelist.push_back(UIButtonType::UPGRADECANCLE);
		typelist.push_back(UIButtonType::ACCELERATE);
	}
	else if(db->status == DBuilding::BuildingStatus::NewBuild)
	{
		typelist.push_back(UIButtonType::INFO);
	}
	else
	{
		switch (type)
		{   
			//采集器
		    case 740001:
			case 740003:
			case 740005:
				typelist.push_back(UIButtonType::INFO);
				typelist.push_back(UIButtonType::UPGRADE);
				typelist.push_back(UIButtonType::COLLECT);
				break;
            
			//仓库
			case 740002:
			case 740004:
			case 740006:
				typelist.push_back(UIButtonType::INFO);
				typelist.push_back(UIButtonType::UPGRADE);
				break;
            
			// 英雄酒馆
			case 740009:
				typelist.push_back(UIButtonType::INFO);
				typelist.push_back(UIButtonType::HERORECRUIT);
				break;
			
			//实验室
			case 740010:
			case 740011:
				 typelist.push_back(UIButtonType::INFO);
				 typelist.push_back(UIButtonType::UPGRADE);
				 typelist.push_back(UIButtonType::ENTER);
                 break;
            
			//工厂
			case 740012:
			case 740013:
			case 740014:
				typelist.push_back(UIButtonType::INFO);
				typelist.push_back(UIButtonType::UPGRADE);
				typelist.push_back(UIButtonType::RECRUIT);
				break;
			
			case 740015:
				typelist.push_back(UIButtonType::INFO);
				typelist.push_back(UIButtonType::UPGRADE);
				typelist.push_back(UIButtonType::HEROCFG);
				break;

		    //小岛基地,房子
			case 740019:
			case 740020:
				typelist.push_back(UIButtonType::INFO);
				break;
			
			//城墙
			case 720001:
				{
				    typelist.push_back(UIButtonType::INFO);
					auto wall = dynamic_cast<UIWall*>(db->getBuilding());
					if (wall->getRowStatus() == UIWall::RowSelectStatus::NOMAL)
					{
						typelist.push_back(UIButtonType::SELECTROW);
					}
					else if (wall->getRowStatus() == UIWall::RowSelectStatus::ROWSELECTE)
					{
						typelist.push_back(UIButtonType::ROTATEROW);
					}
				    typelist.push_back(UIButtonType::UPGRADE);
				}
				break;

			default:
				typelist.push_back(UIButtonType::INFO);
				typelist.push_back(UIButtonType::UPGRADE);
				break;
		}
	}
	
	for (auto ty : typelist )
	{
		vec.pushBack(getButtonFromTypeAndDB(ty,db,callback));
	}
	return vec;
}

UITouchButton * UIButtonFactory::getButtonFromTypeAndDB( UIButtonType type,DBuilding * db,const ccMenuCallback& callback )
{   
	auto btn =  UITouchButton::create("ui/littelbutton.png");
	btn->setTag((int)type);
	auto label = Label::createWithSystemFont("","",20);
	label->setTag(111);
	//TODO 根据资源创建按钮
	switch (type)
	{
	case UIButtonFactory::UIButtonType::INFO:
		label->setString(Configuration::getInstance()->getValue("btn_info").asString());
		break;
	case UIButtonFactory::UIButtonType::UPGRADE:
		label->setString(Configuration::getInstance()->getValue("btn_upgrade").asString());
		break;
	case UIButtonFactory::UIButtonType::UPGRADECANCLE:
		label->setString(Configuration::getInstance()->getValue("btn_upgradecancle").asString());
		break;
	case UIButtonFactory::UIButtonType::ACCELERATE:
		label->setString(Configuration::getInstance()->getValue("btn_acc").asString());
		break;
	case UIButtonFactory::UIButtonType::COLLECT:
		label->setString(Configuration::getInstance()->getValue("btn_collect").asString());
		break;
	case UIButtonFactory::UIButtonType::ENTER:
		label->setString(Configuration::getInstance()->getValue("btn_enter").asString());
		break;
	case UIButtonFactory::UIButtonType::RECRUIT:
		label->setString(Configuration::getInstance()->getValue("btn_recurit").asString());
		break;
	case UIButtonFactory::UIButtonType::HEROCFG:
		label->setString(Configuration::getInstance()->getValue("btn_herocfg").asString());
		break;
	case UIButtonFactory::UIButtonType::HERORECRUIT:
		label->setString(Configuration::getInstance()->getValue("btn_herorecruit").asString());
		break;
	case UIButtonType::SELECTROW:
		label->setString(Configuration::getInstance()->getValue("btn_selectrow").asString());
		break;
	case UIButtonType::ROTATEROW:
		label->setString(Configuration::getInstance()->getValue("btn_rotaterow").asString());
		break;
	default:
		break;
	}

	btn->addChild(label);
	btn->setClickTarget(callback);
	return btn;
}
