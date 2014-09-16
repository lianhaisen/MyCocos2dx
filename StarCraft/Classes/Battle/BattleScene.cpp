#include "BattleScene.h"
#include "AppMacros.h"
#include "GameString.h"
#include "MapHelper.h"
#include "AStar.h"

#include "BuildingCommon.h"
#include "UIRadioButton.h"
#include "UIPlaceBuildingLayer.h"

#include "BattleData.h"
#include "BuildingObserver.h"

#include "SoldierFactory.h"
#include "BuildingData.h"
#include "BuildingConfig.h"
#include "SoldierData.h"
#include "LHSTestScene.h"

static Node *_placeLayer;//TODO 释放资源
//static map<int, BattleLogic *> m_Buildings;
static int frame = 0;

BattleScene::BattleScene(void)
{
	frame = 0;
}

BattleScene::~BattleScene(void)
{
	frame = 0;
	BuildingObserver::release();
	BattleData::release();
}

void BattleScene::onEnter()
{
	log("BattleScene::onEnter()");
	Scene::onEnter();

	//定义一个40ms的定时器，fps = 25
	Director::getInstance()->getScheduler()->schedule(schedule_selector(BattleScene::battleUpdate) , this, SCHEDULE_INTERVAL, false);
	//scheduleUpdate();
	
	auto adm = ArmatureDataManager::getInstance();
	adm->addArmatureFileInfoAsync("soldier/700001/700001.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	adm->addArmatureFileInfoAsync("soldier/700002/700002.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	adm->addArmatureFileInfoAsync("soldier/700003/700003.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	adm->addArmatureFileInfoAsync("soldier/700004/700004.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	adm->addArmatureFileInfoAsync("soldier/700005/700005.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	//adm->addArmatureFileInfoAsync("soldier/700006/700006.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	//adm->addArmatureFileInfoAsync("soldier/700007/700007.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	adm->addArmatureFileInfoAsync("soldier/700008/700008.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	//adm->addArmatureFileInfoAsync("soldier/700009/700009.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	//adm->addArmatureFileInfoAsync("soldier/700010/700010.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	//adm->addArmatureFileInfoAsync("soldier/700011/700011.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	//adm->addArmatureFileInfoAsync("soldier/700012/700012.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	//adm->addArmatureFileInfoAsync("soldier/700013/700013.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	//adm->addArmatureFileInfoAsync("soldier/700014/700014.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	//adm->addArmatureFileInfoAsync("soldier/700015/700015.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
	//adm->addArmatureFileInfoAsync("soldier/700016/700016.ExportJson", this, schedule_selector(BattleScene::dataLoaded));
}

// on "init" you need to initialize your instance
bool BattleScene::init()
{
	log("BattleScene::init()");
    if ( !Scene::init() )
    {
        return false;
    }
	initBattleData();
	createUI();
	createBottomBar();
	
    return true;
}

void BattleScene::initBattleData()
{
	auto bd = BattleData::getInstance();

	auto soldierInCamp = SoldierData::getSoldierDataByType();
	bd->setAttackSoldierInCamp(soldierInCamp);
	//bd->initCoodMap();
}

void BattleScene::battleUpdate(float dt)
{ 
	//log("dt = %f", dt);
	frame++;
}

//创建UI
void BattleScene::createUI()
{	
	//防守方建筑，先用本地数据代替
	auto bd = BuildingData::getBuildingData();
	vector<BattleLogic *> defBuildings;
	auto layer = BuildingCommon::getPlayer3(bd, defBuildings);
	addChild(layer);
	layer->SetDelegate(this);
	_placeLayer = (Node *)layer->getPlaceLayer();

	map<int, BattleLogic*> buildings;
	for (auto item:defBuildings)
	{
		buildings.insert(map<int, BattleLogic*>::value_type(item->getId(), item));
	}

	BattleData::getInstance()->setDefendBuilding(buildings);

	
	//添加按钮
	auto btn = UITouchButton::create("ui/littelbutton.png");
	auto label = Label::createWithSystemFont("back","",20);
	btn->setPosition(Point(btn->getContentSize().width/2, Director::getInstance()->getWinSize().height - btn->getContentSize().height/2));
	btn->setClickTarget(CC_CALLBACK_1(BattleScene::onBackToTsetScence,this));
	btn->addChild(label);
	addChild(btn);

	////为炮塔添加监听
	initTowerObserver();
}

//TODO 屏蔽事件穿透
void BattleScene::createBottomBar()
{
	LayerColor *bottom = LayerColor::create(Color4B(0, 0, 100, 100));
	bottom->setContentSize(Size(WINSIZE.width, 120));
	bottom->setPosition(Point(0, 0));
	addChild(bottom);

	int totalCount = 11;
	int index = 0;
	Vector<MenuItem *> items;
	auto soldierInCamp = BattleData::getInstance()->getAttackSoldierInCamp();
	map<int, vector<DSoldier *>>::iterator it;
	for(it=soldierInCamp.begin(); it!=soldierInCamp.end(); it++)
	{
		MenuItemImage *item = MenuItemImage::create("ui/616.0.png", "ui/617.0.png", "ui/618.0.png", CC_CALLBACK_1(BattleScene::selectArmy, this));
		item->setTag(it->first);
		items.pushBack(item);
		index++;

		//head
		Size size = item->getContentSize();
		char img[20];
		sprintf(img, "ui/%d.png", it->first / 100);
		auto head = Sprite::create(img);
		head->setAnchorPoint(Point(0.5, 0.5));
		head->setPosition(size.width/2, size.height/2);
		item->addChild(head);
		
		char text[20];
		sprintf(text, "X %d", it->second.size());
		TTFConfig config("fonts/tahoma.ttf",16);
		auto count = Label::createWithTTF(config, text, TextHAlignment::LEFT);
		count->setPosition(size.width/2, size.height - count->getContentSize().height);
		item->addChild(count);
		//初始化为第一个兵种
		if (m_selectedSoldierType <= 0)
		{
			m_selectedSoldierType = it->first;
		}
		
	}
	for (int i = index; i < totalCount; i++)
	{
		MenuItemImage *item = MenuItemImage::create("ui/616.0.png", "ui/617.0.png", "ui/618.0.png", CC_CALLBACK_1(BattleScene::selectArmy, this));
		item->setEnabled(false);
		items.pushBack(item);
	}

	Size bottomSize = bottom->getContentSize();
	Sprite *spItem = Sprite::create("ui/618.0.png");
	Size itemSize = spItem->getContentSize();
	float padding = (bottomSize.width - itemSize.width * totalCount) / (totalCount + 1);

	UIRadioButton *radio = UIRadioButton::createWithArray(items);
	radio->alignItemsHorizontallyWithPadding(padding);
	radio->setPosition(Point(bottomSize.width / 2, bottomSize.height / 2));
	radio->setSelectedIndex(0);
	bottom->addChild(radio);
}

// TODO 默认选中的兵种需要默认赋值
void BattleScene::selectArmy(Ref* sender)
{
	 MenuItem* item = (MenuItem*)sender;
	 m_selectedSoldierType = item->getTag();
}

//----------------UILongPressOrClickDelegate begin--------------------------
void BattleScene::doLongPress( PointArray* array )
{   
	//static int count = 0;
	//auto parent = (UIPanZoomLayer *)this->getChildByTag(100);
	//for (int i = 0; i < array->count(); i++)
	//{
	//	auto armature = Armature::create("pangzi");
	//	armature->getAnimation()->playWithIndex(0);
	//	armature->setPosition(array->getControlPointAtIndex(1));
	//	armature->setScale(0.25f);
	//	parent->getPlaceLayer()->addChild(armature);
	//	count++;
	//	log("armature count = %d", count);
	//}
	

}

void BattleScene::doClick( PointArray* array )
{
	if (array != NULL && array->count() > 0)
	{
		//TODO:判断是否下兵点
		addArmy(array->getControlPointAtIndex(0));
	}
}

bool BattleScene::checkIsCanClickOrLongPress( PointArray* array )
{
	return true;
}
//----------------UILongPressOrClickDelegate begin--------------------------

void BattleScene::addArmy(Point point)
{
	Coord coord = MapHelper::convertPointToCoord(point);
	auto soldierInCamp = BattleData::getInstance()->getAttackSoldierInCamp();
	map<int, vector<DSoldier *>>::iterator itCamp = soldierInCamp.find(m_selectedSoldierType);
	if (itCamp != soldierInCamp.end())
	{
		if (!itCamp->second.empty())
		{
			DSoldier *ds = itCamp->second.front();
			Soldier *soldier = SoldierFactory::createSoldier(ds->id);
			soldier->setCoord(coord);
			soldier->setPosition(MapHelper::convertCoordToPoint(coord));

			vector<BattleLogic*> candidate;
			soldier->findCandidateTarget(candidate);
			if (candidate.empty())
			{
				soldier->getBody()->getAnimation()->play("01_stand01", -1, 1);
			}
			else
			{
				auto target = soldier->findTargetFromCandidate(candidate);
				soldier->setTarget(target);
				soldier->setStatus(e_battle_status_moveing);
				soldier->doMove();
			}

			_placeLayer->addChild(soldier);
			soldier->startLogic();
			BattleData::getInstance()->addAttackSoldierInBattle(soldier);
			BattleData::getInstance()->deleteSoldierInCamp(soldier);
		}
	}
}

Node* BattleScene::getPlaceLayer()
{
	return _placeLayer;
}

void BattleScene::dataLoaded(float percent)
{
	//TTFConfig config("fonts/tahoma.ttf",16);
	//auto label = Label::createWithTTF(config,"",TextHAlignment::LEFT);
	//label->setTag(11);
	//label->setPosition(WINSIZE.width/2, WINSIZE.height/2);
	//this->addChild(label);
	//if (label)
	//{
	//	char pszPercent[255];
	//	sprintf(pszPercent, "dataLoaded %f%%", percent * 100);
	//	log(pszPercent);
	//	label->setString(pszPercent);
	//}
}

void BattleScene::onFrameEvent(Bone *bone, const char *evt, int originFrameIndex, int currentFrameIndex)  
{  
	//if(strcmp(evt,"110") == 0)  
	//{  
	//	ActionInterval *action =  ShatteredTiles3D::create(0.8f, Size(30,30), 5, false);   
	//	this->runAction(action);  
	//}  
	if (currentFrameIndex == 5)
	{
		log("currentFrameIndex == 5");
	}
}  

void BattleScene::animationEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	if (movementType == COMPLETE)
	{
		log("animationEvent movementID = ", movementID);
		if (movementID == "dapaolv01_19")
		{
			armature->setScaleX(-1);
			std::vector<string> names;
			names.push_back("dapaolv01_19");
			names.push_back("dapaolv01_18");
			names.push_back("dapaolv01_17");
			names.push_back("dapaolv01_16");
			names.push_back("dapaolv01_15");
			armature->getAnimation()->playWithNames(names, 1, false);
		}
	}
}

void BattleScene::initTowerObserver()
{
	auto buildings = BattleData::getInstance()->getDefendBuilding();
	map<int, BattleLogic *>::iterator it;
	for(it=buildings.begin(); it!=buildings.end(); it++)
	{
		auto item = it->second;
		Coord coord = item->getMidCoord();
		int type = item->getType();
		auto bc = BuildingConfig::getBuildingConfig(type);
		if (bc->building_type == 3)//塔防//TODO:转换为枚举
		{
			int minDis = 0;
			if (bc->min_range > 0)
			{
				minDis = pow(bc->min_range + 1, 2);
			}
			int maxDis = pow(bc->max_range + 1, 2);
			int range = bc->max_range + 1;
			//int count = 0;
			for (int i=-range; i<range; i++)
			{
				for (int j=-range; j<range; j++)
				{
					if (coord.x + i >= 0 && coord.x + i <= MAX_GRID_COUNT && coord.y + j >= 0 && coord.y + j <= MAX_GRID_COUNT)
					{
						int dis = pow(i, 2) + pow(j, 2);
						if (dis >= minDis && dis <= maxDis)
						{
							//key = x*MAX_GRID_COUNT+y
							BuildingObserver::getInstance()->addObserver((coord.x+i)*MAX_GRID_COUNT+(coord.y+j), item->getId());

							//auto dian = Sprite::create("ui/1018.0.png");
							//dian->setPosition(MapHelper::convertCoordToPoint(Coord(coord.x+i, coord.y+j)));
							//_placeLayer->addChild(dian);
							//count++;
							//log("count = %d", count);
						}
					}
				}
			}
		}
	}
}

void BattleScene::onBackToTsetScence( Ref * ref )
{
	Director::getInstance()->replaceScene(TestScene::create());
}












