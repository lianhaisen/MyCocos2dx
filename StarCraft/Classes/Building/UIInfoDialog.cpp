#include "UIInfoDialog.h"
#include "BuildingCommon.h"
#include "BuildingConfig.h"
#include "CampTableViewCell.h"
#include "BarrackData.h"
#include "BuildingFunc.h"

#define CON Configuration::getInstance()

UIInfoDlg::UIInfoDlg()
{
	_db = nullptr;
	_tableview = nullptr;
}

UIInfoDlg::~UIInfoDlg()
{
 
}

 UIInfoDlg* UIInfoDlg::CreateFromDB(DBuilding * db)
 {
	 UIInfoDlg * ed = UIInfoDlg::create();
	 ed->_db = db;

	 ed->setInfoPic(Sprite::create(BuildingCommon::getPicFromId(db->type)));
	 ed->getInfoPic()->setScale(0.5f);
	 
	 auto cfg = db->getCurBindCfg();
	 auto str = String::createWithFormat(CON->getValue("des").asString().c_str(),cfg->name.c_str(),cfg->id % 100);
	 ed->setTitle(str->getCString());
     
	 ed->createTimer();
	 return ed;
 }

void UIInfoDlg::createTimer()
{
	setTimerNode(Node::create());
	if (_db->type / 100 == 740007)
	{
		//军营
		int curpu = 0;
		auto item = BarrackData::getBarrackFromId(_db->id);
		if (item)
		{
			curpu = item->curpopulation;
		}
	    createTimer(String::createWithFormat(CON->getValue("population").asString().c_str(),curpu,_db->getCurBindCfg()->population)->getCString(),curpu,_db->getCurBindCfg()->population);
		createTimer(String::createWithFormat(CON->getValue("hp").asString().c_str(),_db->getCurBindCfg()->max_hp,_db->getCurBindCfg()->max_hp)->getCString(),_db->getCurBindCfg()->max_hp,_db->getCurBindCfg()->max_hp,-50.0f);
		createCamp();
	}
	else
	{
		createTimer(String::createWithFormat(CON->getValue("storage").asString().c_str(),_db->curpro,_db->getCurBindCfg()->storage)->getCString(),_db->curpro,_db->getCurBindCfg()->storage);
		createTimer(String::createWithFormat(CON->getValue("hp").asString().c_str(),_db->getCurBindCfg()->max_hp,_db->getCurBindCfg()->max_hp)->getCString(),_db->getCurBindCfg()->max_hp,_db->getCurBindCfg()->max_hp,-50.0f);
	    setContentText(CON->getValue("testlabel").asString().c_str());
	}
	
}
	
void UIInfoDlg::createTimer(string str,int cur,int max,float offsetY )
{
	auto bg = Sprite::create("ui/processbg2.png");
	bg->setScale(3.0f);
	auto timerbg = Sprite::create("ui/process.png");
	auto timer = ProgressTimer::create(timerbg);
	timer->setScale(3.0f);
	timer->setType(ProgressTimer::Type::BAR);
	timer->setMidpoint(Point(0,100));
	timer->setBarChangeRate(Point(1,0));
	timer->setPercentage(float(cur) / max * 100);


	auto label = Label::createWithSystemFont(str,"",10);
	label->setColor(Color3B::BLACK);
	label->setHorizontalAlignment(TextHAlignment::LEFT);
	label->setVerticalAlignment(TextVAlignment::BOTTOM);


	label->setPosition(Point( 0, bg->getContentSize().height + 10 + offsetY));
	timer->setPositionY(offsetY);
	bg->setPositionY(offsetY);

	getTimerNode()->addChild(bg);
	getTimerNode()->addChild(timer);
	getTimerNode()->addChild(label);
}

void UIInfoDlg::createCamp()
{
	 _tableview = TableView::create(this,Size(getContextNode()->getContentSize().width - 50,150));
	 _tableview->setDirection(ScrollView::Direction::HORIZONTAL);
	 _tableview->setPosition(Point(25,100));
	 _tableview->setDelegate(this);
	 getContextNode()->addChild(_tableview,100);
	 _tableview->reloadData();


}

Size UIInfoDlg::tableCellSizeForIndex( TableView *table, ssize_t idx )
{
	return Size(86 + 10,108);
}

TableViewCell* UIInfoDlg::tableCellAtIndex( TableView *table, ssize_t idx )
{
	auto cell = dynamic_cast<CampTableViewCell*>(table->dequeueCell());
	if (!cell)
	{
		cell = CampTableViewCell::create();
	}

	auto map = BuildingFunc::getInstance()->getAllSoldier();
	if (idx < map.size())
	{   
		int index = 0;
		auto it = map.begin();
		while(index < idx)
		{
			it++;
			index++;
		}
		auto item = it->second;
		cell->setInfo(item->soldierid / 100,item->soldierid  % 100,item->number);
	}
	else
	{   
		cell->disableInfo();
	}

	return cell;
}

ssize_t UIInfoDlg::numberOfCellsInTableView( TableView *table )
{
	return 20;
}
